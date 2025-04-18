/**
 * @file brush
 * @author Krisna Pranav
 * @version 6.0
 * @date 2023-06-28
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

use std::num::NonZeroU32;

use crate::{
    errors::BrushError,
    pipeline::{Pipeline, Vertex},
    Matrix,
};
use glyph_brush::{
    ab_glyph::{Font, FontArc, FontRef, InvalidFont, Rect},
    BrushAction, DefaultSectionHasher, Extra, GlyphCruncher, Section, SectionGlyphIter,
};

pub struct TextBrush<F = FontArc, H = DefaultSectionHasher> {
    inner: glyph_brush::GlyphBrush<Vertex, Extra, F, H>,
    pipeline: Pipeline,
}

impl<F, H> TextBrush<F, H>
where
    F: Font + Sync,
    H: std::hash::BuildHasher,
{
    #[inline]
    pub fn queue<'a, S, I: IntoIterator<Item = S>>(
        &mut self,
        device: &wgpu::Device,
        queue: &wgpu::Queue,
        sections: I,
    ) -> Result<(), BrushError>
    where
        S: Into<std::borrow::Cow<'a, Section<'a>>>,
    {
        for s in sections {
            self.inner.queue(s);
        }

        loop {
            let brush_action = self.inner.process_queued(
                |rect, data| self.pipeline.update_texture(rect, data, queue),
                Vertex::to_vertex,
            );

            match brush_action {
                Ok(action) => {
                    break match action {
                        BrushAction::Draw(vertices) => {
                            self.pipeline.update_vertex_buffer(vertices, device, queue)
                        }
                        BrushAction::ReDraw => (),
                    }
                }

                Err(glyph_brush::BrushError::TextureTooSmall { suggested }) => {
                    if log::log_enabled!(log::Level::Warn) {
                        log::warn!(
                            "Resizing cache texture! This should be avoided \
                            by building TextBrush with BrushBuilder::initial_cache_size() \
                            and providing bigger cache texture dimensions."
                        );
                    }

                    let max_image_dimension = device.limits().max_texture_dimension_2d;
                    let (width, height) = if suggested.0 > max_image_dimension
                        || suggested.1 > max_image_dimension
                    {
                        if self.inner.texture_dimensions().0 < max_image_dimension
                            || self.inner.texture_dimensions().1 < max_image_dimension
                        {
                            (max_image_dimension, max_image_dimension)
                        } else {
                            return Err(BrushError::TooBigCacheTexture(
                                max_image_dimension,
                            ));
                        }
                    } else {
                        suggested
                    };
                    self.pipeline.resize_texture(device, (width, height));
                    self.inner.resize_texture(width, height);
                }
            }
        }
        Ok(())
    }

    #[inline]
    pub fn glyph_bounds<'a, S>(&mut self, section: S) -> Option<Rect>
    where
        S: Into<std::borrow::Cow<'a, Section<'a>>>,
    {
        self.inner.glyph_bounds(section)
    }

    #[inline]
    pub fn glyphs_iter<'a, 'b, S>(&'b mut self, section: S) -> SectionGlyphIter<'b>
    where
        S: Into<std::borrow::Cow<'a, Section<'a>>>,
    {
        self.inner.glyphs(section)
    }

    pub fn fonts(&self) -> &[F] {
        self.inner.fonts()
    }

    #[inline]
    pub fn draw<'pass>(&'pass self, rpass: &mut wgpu::RenderPass<'pass>) {
        self.pipeline.draw(rpass)
    }

    #[inline]
    pub fn resize_view(&self, width: f32, height: f32, queue: &wgpu::Queue) {
        self.update_matrix(crate::ortho(width, height), queue);
    }

    #[inline]
    pub fn update_matrix<M>(&self, matrix: M, queue: &wgpu::Queue)
    where
        M: Into<Matrix>,
    {
        self.pipeline.update_matrix(matrix.into(), queue);
    }
}

#[non_exhaustive]
pub struct BrushBuilder<F, H = DefaultSectionHasher> {
    inner: glyph_brush::GlyphBrushBuilder<F, H>,
    depth_stencil: Option<wgpu::DepthStencilState>,
    multisample: wgpu::MultisampleState,
    multiview: Option<NonZeroU32>,
    matrix: Option<Matrix>,
}

impl BrushBuilder<()> {
    pub fn using_font<F: Font>(font: F) -> BrushBuilder<F> {
        BrushBuilder::using_fonts(vec![font])
    }

    pub fn using_font_bytes(data: &[u8]) -> Result<BrushBuilder<FontRef>, InvalidFont> {
        let font = FontRef::try_from_slice(data)?;
        Ok(BrushBuilder::using_fonts(vec![font]))
    }

    pub fn using_font_bytes_vec(
        data: &[u8],
    ) -> Result<BrushBuilder<FontRef>, InvalidFont> {
        let font = FontRef::try_from_slice(data)?;
        Ok(BrushBuilder::using_fonts(vec![font]))
    }

    pub fn using_fonts<F: Font>(fonts: Vec<F>) -> BrushBuilder<F> {
        BrushBuilder {
            inner: glyph_brush::GlyphBrushBuilder::using_fonts(fonts),
            depth_stencil: None,
            multisample: wgpu::MultisampleState::default(),
            multiview: None,
            matrix: None,
        }
    }
}

impl<F, H> BrushBuilder<F, H>
where
    F: Font,
    H: std::hash::BuildHasher,
{
    glyph_brush::delegate_glyph_brush_builder_fns!(inner);

    pub fn with_matrix<M>(mut self, matrix: M) -> Self
    where
        M: Into<Matrix>,
    {
        self.matrix = Some(matrix.into());
        self
    }

    pub fn with_multisample(mut self, multisample: wgpu::MultisampleState) -> Self {
        self.multisample = multisample;
        self
    }

    pub fn with_multiview(mut self, multiview: NonZeroU32) -> Self {
        self.multiview = Some(multiview);
        self
    }

    pub fn with_depth_stencil(
        mut self,
        depth_stencil: Option<wgpu::DepthStencilState>,
    ) -> BrushBuilder<F, H> {
        self.depth_stencil = depth_stencil;
        self
    }

    pub fn build(
        self,
        device: &wgpu::Device,
        render_width: u32,
        render_height: u32,
        render_format: wgpu::TextureFormat,
    ) -> TextBrush<F, H> {
        let inner = self.inner.build();

        let matrix = self
            .matrix
            .unwrap_or_else(|| crate::ortho(render_width as f32, render_height as f32));

        let pipeline = Pipeline::new(
            device,
            render_format,
            self.depth_stencil,
            self.multisample,
            self.multiview,
            inner.texture_dimensions(),
            matrix,
        );

        TextBrush { inner, pipeline }
    }
}
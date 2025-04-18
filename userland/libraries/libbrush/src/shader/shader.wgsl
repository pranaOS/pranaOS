struct VertexInput {
    @builtin(vertex_index) vertex_index: u32,
    @location(0) top_left: vec3<f32>,
    @location(1) bottom_right: vec2<f32>,
    @location(2) tex_top_left: vec2<f32>,
    @location(3) tex_bottom_right: vec2<f32>,
    @location(4) color: vec4<f32>,
}

struct Matrix {
    v: mat4x4<f32>,
}

@group(0) @binding(0)
var<uniform> ortho: Matrix;

struct VertexOutput {
    @builtin(position) clip_position: vec4<f32>,
    @location(0) tex_pos: vec2<f32>,
    @location(1) color: vec4<f32>,
}

@vertex
fn vs_main(in: VertexInput) -> VertexOutput {
    var out: VertexOutput;

    var pos: vec2<f32>;
    var left: f32 = in.top_left.x;
    var right: f32 = in.bottom_right.x;
    var top: f32 = in.top_left.y;
    var bottom: f32 = in.bottom_right.y;

    switch (in.vertex_index) {
        case 0u: {
            pos = vec2<f32>(left, top);
            out.tex_pos = in.tex_top_left;
            break;
        }
        case 1u: {
            pos = vec2<f32>(right, top);
            out.tex_pos = vec2<f32>(in.tex_bottom_right.x, in.tex_top_left.y);
            break;
        }
        case 2u: {
            pos = vec2<f32>(left, bottom);
            out.tex_pos = vec2<f32>(in.tex_top_left.x, in.tex_bottom_right.y);
            break;
        }
        case 3u: {
            pos = vec2<f32>(right, bottom);
            out.tex_pos = in.tex_bottom_right;
            break;
        }
        default: {}
    }

    out.clip_position = ortho.v * vec4<f32>(pos, in.top_left.z, 1.0);
    out.color = in.color;
    return out;
}

@group(0) @binding(1)
var texture: texture_2d<f32>;
@group(0) @binding(2)
var tex_sampler: sampler;

@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4<f32> {
    var alpha: f32 = textureSample(texture, tex_sampler, in.tex_pos).r;

    return vec4<f32>(in.color.rgb, in.color.a * alpha);
}
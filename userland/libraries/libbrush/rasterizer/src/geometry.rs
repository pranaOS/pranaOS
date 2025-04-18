/**
 * @file geometry.rs
 * @author Krisna Pranav
 * @version 6.0
 * @date 2023-06-28
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#[cfg(all(feature = "libm", not(feature = "std")))]
use crate::nostd_float::FloatExt;

#[derive(Clone, Copy, Default, PartialEq, PartialOrd)]
pub struct Point {
    pub x: f32,
    pub y: f32,
}

impl core::fmt::Debug for Point {
    fn fmt(&self, f: &mut core::fmt::Formatter<'_>) -> core::fmt::Result {
        write!(f, "point({:?}, {:?})", self.x, self.y)
    }
}

impl Point {
    #[inline]
    pub(crate) fn distance_to(self, other: Point) -> f32 {
        let d = other - self;
        (d.x * d.x + d.y * d.y).sqrt()
    }
}

#[inline]
pub fn point(x: f32, y: f32) -> Point {
    Point { x, y }
}

pub(crate) fn lerp(t: f32, p0: Point, p1: Point) -> Point {
    point(p0.x + t * (p1.x - p0.x), p0.y + t * (p1.y - p0.y))
}

impl core::ops::Sub for Point {
    type Output = Point;

    #[inline]
    fn sub(self, rhs: Point) -> Point {
        point(self.x - rhs.x, self.y - rhs.y)
    }
}

impl core::ops::Add for Point {
    type Output = Point;

    #[inline]
    fn add(self, rhs: Point) -> Point {
        point(self.x + rhs.x, self.y + rhs.y)
    }
}

impl core::ops::AddAssign for Point {
    #[inline]
    fn add_assign(&mut self, other: Self) {
        self.x += other.x;
        self.y += other.y;
    }
}

impl core::ops::SubAssign for Point {
    #[inline]
    fn sub_assign(&mut self, other: Self) {
        self.x -= other.x;
        self.y -= other.y;
    }
}

impl<F: Into<f32>> From<(F, F)> for Point {
    #[inline]
    fn from((x, y): (F, F)) -> Self {
        point(x.into(), y.into())
    }
}

impl<F: Into<f32>> From<[F; 2]> for Point {
    #[inline]
    fn from([x, y]: [F; 2]) -> Self {
        point(x.into(), y.into())
    }
}

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn distance_to() {
        let distance = point(0.0, 0.0).distance_to(point(3.0, 4.0));
        assert!((distance - 5.0).abs() <= f32::EPSILON);
    }
}
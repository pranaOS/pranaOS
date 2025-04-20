/**
 * @file error.rs
 * @author Krisna Pranav
 * @version 6.0
 * @date 2023-06-28
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

use std::io;

use codespan_reporting::diagnostic::{Diagnostic, Label};
use codespan_reporting::files::SimpleFile;
use codespan_reporting::term;
use termcolor::WriteColor;
use thiserror::Error;

use crate::types::{Span, Spanned};

pub type Result<T, E = ErrorS> = std::result::Result<T, E>;
pub type ErrorS = Spanned<Error>;

impl AsDiagnostic for Error {
    fn as_diagnostic(&self, span: &Span) -> Diagnostic<()> {
        match self {
        }
    }
}

macro_rules! impl_from_error {
    () => {};
}

#[derive(Debug, Error, Eq, PartialEq)]
pub enum Error {
}

#[derive(Debug, Error, Eq, PartialEq)]
pub enum AttributeError {
}

impl AsDiagnostic for AttributeError {
    fn as_diagnostic(&self) -> Diagnostic<()> {
        Diagnostic::error().with_message(format!("{:?}", self))
    }
}

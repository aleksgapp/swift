//===--- AccessScope.h - Swift Expression Handle -----------------*- C++ -*-===//
//
// This source file is part of the Swift.org open source project
//
// Copyright (c) 2014 - 2016 Apple Inc. and the Swift project authors
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See http://swift.org/LICENSE.txt for license information
// See http://swift.org/CONTRIBUTORS.txt for the list of Swift project authors
//
//===----------------------------------------------------------------------===//
//
// This file defines the AccessScope class.
//
//===----------------------------------------------------------------------===//

#ifndef SWIFT_ACCESSSCOPE_H
#define SWIFT_ACCESSSCOPE_H

#include "llvm/ADT/PointerIntPair.h"
#include "swift/AST/AttrKind.h"
#include "swift/AST/DeclContext.h"

namespace swift {

/// AccessScope - Provides an indirection for expressions, so both a type and a
/// pattern can point at the same expression during type-checking.
class AccessScope {
  /// \brief The expression along with a bit saying whether this expression
  /// was already type-checked (or not).
  llvm::PointerIntPair<const DeclContext *, 1, bool> Value;
public:
  AccessScope(const DeclContext *DC = nullptr, bool isPrivate = false)
    : Value(DC, isPrivate) {}

  static const AccessScope INVALID;

  bool operator==(const AccessScope RHS) const { return Value == RHS.Value; }
  bool operator!=(const AccessScope RHS) const { return !(*this == RHS); }

  bool isPublic() const { return !Value.getPointer(); }

  const DeclContext *getDeclContext() const { return Value.getPointer(); }

  bool isInvalid() const { return *this == INVALID; }

  /// \brief Determine whether the referenced expression has already been
  /// type-checked.
  bool isPrivate() const { return Value.getInt(); }

  bool isModuleContext() const;
  bool isModuleDecl() const;
  bool isChildOf(const AccessScope AS) const;

  /// Returns the access level associated with \p accessScope, for diagnostic
  /// purposes.
  ///
  /// \sa ValueDecl::getFormalAccessScope
  Accessibility accessibilityForDiagnostics() const;
};

} // end namespace swift

#endif

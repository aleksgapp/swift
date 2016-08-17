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

#include "swift/AST/TypeAlignments.h"
#include "llvm/Support/DataTypes.h"
#include "llvm/ADT/PointerIntPair.h"
#include "swift/AST/DeclContext.h"

namespace swift {
  class ASTContext;

/// AccessScope - Provides an indirection for expressions, so both a type and a
/// pattern can point at the same expression during type-checking.
class alignas(8) AccessScope {
  /// \brief The expression along with a bit saying whether this expression
  /// was already type-checked (or not).
  llvm::PointerIntPair<const DeclContext *, 1, bool> Value;
private:
  AccessScope(const DeclContext *DC, bool isPrivate = false)
    : Value(DC, isPrivate) {}

  void *operator new(size_t Bytes) throw() = delete;
  void operator delete(void *Data) throw() = delete;
  void *operator new(size_t Bytes, void *Mem) throw() = delete;
  void *operator new(size_t Bytes, ASTContext &C,
                     unsigned Alignment = 8);
public:

  bool operator==(const AccessScope &RHS) const { return Value == RHS.Value; }
  bool operator!=(const AccessScope &RHS) const { return !(*this == RHS); }

  const DeclContext *getDeclContext() const { return Value.getPointer(); }

  /// \brief Determine whether the referenced expression has already been
  /// type-checked.
  bool isPrivate() const { return Value.getInt(); }

  bool isModule() const { return getDeclContext()->isModuleContext(); };

  bool isChildScopeOf(const AccessScope *AS) const {
      return getDeclContext()->isChildContextOf(AS->getDeclContext());
  }

  static bool isEqual(const AccessScope *LHS, const AccessScope *RHS) {
      if (!LHS && !RHS)
          return true;
      if (!LHS || !RHS)
        return false;
      return *LHS == *RHS;
  }

  static const AccessScope *get(const DeclContext *DC, bool isPrivate = false);
};

} // end namespace swift

#endif

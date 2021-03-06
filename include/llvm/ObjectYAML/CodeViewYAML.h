//===- CodeViewYAML.cpp - CodeView YAMLIO implementation ------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines classes for handling the YAML representation of CodeView
// Debug Info.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_OBJECTYAML_CODEVIEWYAML_H
#define LLVM_OBJECTYAML_CODEVIEWYAML_H

#include "llvm/DebugInfo/CodeView/CodeView.h"
#include "llvm/DebugInfo/CodeView/DebugSubsection.h"
#include "llvm/DebugInfo/CodeView/SymbolDeserializer.h"
#include "llvm/DebugInfo/CodeView/SymbolRecord.h"
#include "llvm/DebugInfo/CodeView/SymbolSerializer.h"
#include "llvm/DebugInfo/CodeView/TypeDeserializer.h"
#include "llvm/DebugInfo/CodeView/TypeRecord.h"
#include "llvm/DebugInfo/CodeView/TypeSerializer.h"
#include "llvm/DebugInfo/CodeView/TypeTableBuilder.h"
#include "llvm/ObjectYAML/YAML.h"

namespace llvm {
namespace CodeViewYAML {
namespace detail {
struct C13FragmentBase;
struct LeafRecordBase;
struct MemberRecordBase;
struct SymbolRecordBase;
}

namespace detail {
struct MemberRecordBase;
struct LeafRecordBase;
struct SymbolRecordBase;
}

struct SourceLineEntry {
  uint32_t Offset;
  uint32_t LineStart;
  uint32_t EndDelta;
  bool IsStatement;
};

struct SourceColumnEntry {
  uint16_t StartColumn;
  uint16_t EndColumn;
};

struct SourceLineBlock {
  StringRef FileName;
  std::vector<SourceLineEntry> Lines;
  std::vector<SourceColumnEntry> Columns;
};

struct HexFormattedString {
  std::vector<uint8_t> Bytes;
};

struct SourceFileChecksumEntry {
  StringRef FileName;
  codeview::FileChecksumKind Kind;
  HexFormattedString ChecksumBytes;
};

struct SourceLineInfo {
  uint32_t RelocOffset;
  uint32_t RelocSegment;
  codeview::LineFlags Flags;
  uint32_t CodeSize;

  std::vector<SourceLineBlock> Blocks;
};

struct InlineeSite {
  uint32_t Inlinee;
  StringRef FileName;
  uint32_t SourceLineNum;
  std::vector<StringRef> ExtraFiles;
};

struct InlineeInfo {
  bool HasExtraFiles;
  std::vector<InlineeSite> Sites;
};

struct SourceFileInfo {
  std::vector<SourceFileChecksumEntry> FileChecksums;
  std::vector<SourceLineInfo> LineFragments;
  std::vector<InlineeInfo> Inlinees;
};

struct MemberRecord {
  std::shared_ptr<detail::MemberRecordBase> Member;
};

struct LeafRecord {
  std::shared_ptr<detail::LeafRecordBase> Leaf;

  codeview::CVType toCodeViewRecord(BumpPtrAllocator &Allocator) const;
  static Expected<LeafRecord> fromCodeViewRecord(codeview::CVType Type);
};

struct SymbolRecord {
  std::shared_ptr<detail::SymbolRecordBase> Symbol;

  codeview::CVSymbol toCodeViewSymbol(BumpPtrAllocator &Allocator) const;
  static Expected<SymbolRecord> fromCodeViewSymbol(codeview::CVSymbol Symbol);
};

struct C13DebugSection {
  std::vector<detail::C13FragmentBase> Fragments;
};
} // namespace CodeViewYAML
} // namespace llvm

LLVM_YAML_DECLARE_MAPPING_TRAITS(CodeViewYAML::LeafRecord)
LLVM_YAML_DECLARE_MAPPING_TRAITS(CodeViewYAML::MemberRecord)
LLVM_YAML_DECLARE_MAPPING_TRAITS(CodeViewYAML::SymbolRecord)
LLVM_YAML_DECLARE_MAPPING_TRAITS(CodeViewYAML::SourceFileInfo)

LLVM_YAML_IS_SEQUENCE_VECTOR(CodeViewYAML::LeafRecord)
LLVM_YAML_IS_SEQUENCE_VECTOR(CodeViewYAML::MemberRecord)
LLVM_YAML_IS_SEQUENCE_VECTOR(CodeViewYAML::SymbolRecord)

#endif

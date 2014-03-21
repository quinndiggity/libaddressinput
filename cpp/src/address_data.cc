// Copyright (C) 2014 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <libaddressinput/address_data.h>

#include <libaddressinput/address_field.h>
#include <libaddressinput/util/basictypes.h>

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <functional>
#include <string>
#include <vector>

namespace i18n {
namespace addressinput {

namespace {

// Mapping from AddressField value to pointer to AddressData member.
const std::string AddressData::*kStringField[] = {
  &AddressData::region_code,
  &AddressData::administrative_area,
  &AddressData::locality,
  &AddressData::dependent_locality,
  &AddressData::sorting_code,
  &AddressData::postal_code,
  NULL,
  &AddressData::recipient
};

// Mapping from AddressField value to pointer to AddressData member.
const std::vector<std::string> AddressData::*kVectorStringField[] = {
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  &AddressData::address_line,
  NULL
};

COMPILE_ASSERT(arraysize(kStringField) == arraysize(kVectorStringField),
               field_mapping_array_size_mismatch);

}  // namespace

bool AddressData::IsFieldEmpty(AddressField field) const {
  assert(field >= 0);
  assert(static_cast<size_t>(field) < arraysize(kStringField));
  if (kStringField[field] != NULL) {
    const std::string& value = GetFieldValue(field);
    return value.empty();
  } else {
    const std::vector<std::string>& value = GetRepeatedFieldValue(field);
    return std::find_if(value.begin(), value.end(),
                        std::not1(std::mem_fun_ref(&std::string::empty))) ==
           value.end();
  }
}

const std::string& AddressData::GetFieldValue(
    AddressField field) const {
  assert(field >= 0);
  assert(static_cast<size_t>(field) < arraysize(kStringField));
  assert(kStringField[field] != NULL);
  return this->*kStringField[field];
}

const std::vector<std::string>& AddressData::GetRepeatedFieldValue(
    AddressField field) const {
  assert(field >= 0);
  assert(static_cast<size_t>(field) < arraysize(kVectorStringField));
  assert(kVectorStringField[field] != NULL);
  return this->*kVectorStringField[field];
}

}  // namespace addressinput
}  // namespace i18n
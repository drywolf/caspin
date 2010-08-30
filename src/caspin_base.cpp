/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is [Open Source Virtual Machine].
 *
 * The Initial Developer of the Original Code is
 * Adobe System Incorporated.
 * Portions created by the Initial Developer are Copyright (C) 2008
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *   Adobe AS3 Team
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

/* machine generated file -- do not edit */

namespace avmplus { namespace NativeID {

const uint32_t caspin_base_abc_class_count = 1;
const uint32_t caspin_base_abc_script_count = 2;
const uint32_t caspin_base_abc_method_count = 7;
const uint32_t caspin_base_abc_length = 349;

/* thunks (1 unique signatures, 1 total) */

#ifndef VMCFG_INDIRECT_NATIVE_THUNKS

AvmBox caspin_System_trace_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv)
{
    enum {
        argoff0 = 0
        , argoff1 = argoff0 + AvmThunkArgSize_AvmObject
    };
    (void)argc;
    (void)env;
    ::csp::SystemClass* const obj = (::csp::SystemClass*)AvmThunkUnbox_AvmReceiver(AvmObject, argv[argoff0]);
    obj->trace(
        (ArrayObject*)AvmThunkUnbox_AvmObject(argv[argoff1])
    );
    return kAvmThunkUndefined;
}

#else // VMCFG_INDIRECT_NATIVE_THUNKS

// caspin_System_trace
AvmBox caspin_base_v2a_oo_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv)
{
    enum {
        argoff0 = 0
        , argoff1 = argoff0 + AvmThunkArgSize_AvmObject
    };
    (void)argc;
    (void)env;
    ::csp::SystemClass* const obj = (::csp::SystemClass*)AvmThunkUnbox_AvmReceiver(AvmObject, argv[argoff0]);
    obj->trace(
        (ArrayObject*)AvmThunkUnbox_AvmObject(argv[argoff1])
    );
    return kAvmThunkUndefined;
}

#endif // VMCFG_INDIRECT_NATIVE_THUNKS

class SlotOffsetsAndAsserts
{
private:
    static uint32_t getSlotOffset(Traits* t, int nameId);
public:
    static const uint16_t s_slotsOffsetSystemClass = 0;
    static const uint16_t s_slotsOffsetScriptObject = 0;
    #ifdef DEBUG
    static void doSystemClassAsserts(Traits* cTraits, Traits* iTraits);
    #endif
};
#ifdef DEBUG
REALLY_INLINE void SlotOffsetsAndAsserts::doSystemClassAsserts(Traits* cTraits, Traits* iTraits)
{
    (void)cTraits; (void)iTraits;
    // MMGC_STATIC_ASSERT(sizeof(::csp::SystemClass::EmptySlotsStruct_SystemClass) >= 0);
}
#endif // DEBUG

AVMTHUNK_NATIVE_CLASS_GLUE(SystemClass, ::csp::SystemClass, SlotOffsetsAndAsserts::doSystemClassAsserts)

AVMTHUNK_BEGIN_NATIVE_TABLES(caspin_base)

    AVMTHUNK_BEGIN_NATIVE_METHODS(caspin_base)
        AVMTHUNK_NATIVE_METHOD(caspin_System_trace, ::csp::SystemClass::trace)
    AVMTHUNK_END_NATIVE_METHODS()

    AVMTHUNK_BEGIN_NATIVE_CLASSES(caspin_base)
        AVMTHUNK_NATIVE_CLASS(abcclass_caspin_System, SystemClass, ::csp::SystemClass, SlotOffsetsAndAsserts::s_slotsOffsetSystemClass, ScriptObject, SlotOffsetsAndAsserts::s_slotsOffsetScriptObject)
    AVMTHUNK_END_NATIVE_CLASSES()

AVMTHUNK_END_NATIVE_TABLES()

AVMTHUNK_DEFINE_NATIVE_INITIALIZER(caspin_base)

/* abc */
const uint8_t caspin_base_abc_data[349] = {
  16,   0,  46,   0,   0,   0,   0,  16,   0,   5, 116, 114,  97,  99, 101,  16,
  99,  97, 115, 112, 105, 110,  95,  98,  97, 115, 101,  46,  97, 115,  36,  49,
  13,  99,  97, 115, 112, 105, 110,  58,  83, 121, 115, 116, 101, 109,   4, 118,
 111, 105, 100,   5,  65, 114, 114,  97, 121,   6,  99,  97, 115, 112, 105, 110,
   6,  83, 121, 115, 116, 101, 109,   6,  79,  98, 106, 101,  99, 116,   6, 110,
  97, 116, 105, 118, 101,   3,  99, 108, 115,  18,  58,  58,  99, 115, 112,  58,
  58,  83, 121, 115, 116, 101, 109,  67, 108,  97, 115, 115,   7, 109, 101, 116,
 104, 111, 100, 115,   4,  97, 117, 116, 111,   5, 112, 114, 105, 110, 116,   8,
   5,   3,  22,   1,  23,   1,   5,   4,  22,   7,  24,   4,  23,   7,   3,   3,
   1,   2,   3,   4,   1,   2,   5,   7,   9,   9,   2,   1,   7,   2,   5,   7,
   2,   6,   7,   2,   2,   7,   5,   8,   7,   2,   9,   9,   9,   2,   7,   2,
  15,   7,   0,   0,   1,   0,   0,   0,   1,   4,   0,   0,   1,   4,   0,   0,
   1,   0,   1,   2,   3,   1,  32,   0,   0,   1,   0,   0,   0,   1,   0,   1,
  10,   2,  11,  13,  12,  14,   1,   5,   6,   9,   6,   0,   5,   0,   3,   1,
   4,  17,   3,   4,   2,   6,   3,   5,  68,   1,   0,   1,   0,   8,   1,   1,
   2,   4,   1,   2,   1,   0,   0,   6,   0,   1,   1,   1,   2,   3, 208,  48,
  71,   0,   0,   1,   2,   2,   1,   2,  11, 208,  48, 100, 108,   1, 209,  70,
   1,   1,  41,  71,   0,   0,   2,   2,   2,   1,   2,  11, 208,  48, 100, 108,
   1, 209,  70,   1,   1,  41,  71,   0,   0,   3,   1,   1,   3,   4,   3, 208,
  48,  71,   0,   0,   5,   1,   1,   4,   5,   6, 208,  48, 208,  73,   0,  71,
   0,   0,   6,   2,   1,   1,   3,  19, 208,  48, 101,   0,  93,   6, 102,   6,
  48,  93,   7, 102,   7,  88,   0,  29, 104,   5,  71,   0,   0};

} }

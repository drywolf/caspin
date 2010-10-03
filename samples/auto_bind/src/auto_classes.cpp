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

const uint32_t auto_classes_abc_class_count = 1;
const uint32_t auto_classes_abc_script_count = 2;
const uint32_t auto_classes_abc_method_count = 7;
const uint32_t auto_classes_abc_length = 645;

/* thunks (2 unique signatures, 2 total) */

#ifndef VMCFG_INDIRECT_NATIVE_THUNKS

AvmBox flash_geom_ColorTransform_color_get_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv)
{
    enum {
        argoff0 = 0
    };
    (void)argc;
    (void)env;
    ::flash::geom::ColorTransform* const obj = (::flash::geom::ColorTransform*)AvmThunkUnbox_AvmReceiver(AvmObject, argv[argoff0]);
    int32_t const ret = obj->get_color();
    return (AvmBox) ret;
}
AvmBox flash_geom_ColorTransform_color_set_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv)
{
    enum {
        argoff0 = 0
        , argoff1 = argoff0 + AvmThunkArgSize_AvmObject
    };
    (void)argc;
    (void)env;
    ::flash::geom::ColorTransform* const obj = (::flash::geom::ColorTransform*)AvmThunkUnbox_AvmReceiver(AvmObject, argv[argoff0]);
    obj->set_color(
        AvmThunkUnbox_int32_t(argv[argoff1])
    );
    return kAvmThunkUndefined;
}

#else // VMCFG_INDIRECT_NATIVE_THUNKS

// flash_geom_ColorTransform_color_get
AvmBox auto_classes_i2a_o_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv)
{
    enum {
        argoff0 = 0
    };
    (void)argc;
    (void)env;
    ::flash::geom::ColorTransform* const obj = (::flash::geom::ColorTransform*)AvmThunkUnbox_AvmReceiver(AvmObject, argv[argoff0]);
    int32_t const ret = obj->get_color();
    return (AvmBox) ret;
}

// flash_geom_ColorTransform_color_set
AvmBox auto_classes_v2a_oi_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv)
{
    enum {
        argoff0 = 0
        , argoff1 = argoff0 + AvmThunkArgSize_AvmObject
    };
    (void)argc;
    (void)env;
    ::flash::geom::ColorTransform* const obj = (::flash::geom::ColorTransform*)AvmThunkUnbox_AvmReceiver(AvmObject, argv[argoff0]);
    obj->set_color(
        AvmThunkUnbox_int32_t(argv[argoff1])
    );
    return kAvmThunkUndefined;
}

#endif // VMCFG_INDIRECT_NATIVE_THUNKS

class SlotOffsetsAndAsserts
{
private:
    static uint32_t getSlotOffset(Traits* t, int nameId);
public:
    static const uint16_t s_slotsOffsetColorTransformClass = 0;
    static const uint16_t s_slotsOffsetColorTransform = offsetof(::flash::geom::ColorTransform, m_slots_ColorTransform);
    #ifdef DEBUG
    static void doColorTransformClassAsserts(Traits* cTraits, Traits* iTraits);
    #endif
};
#ifdef DEBUG
REALLY_INLINE void SlotOffsetsAndAsserts::doColorTransformClassAsserts(Traits* cTraits, Traits* iTraits)
{
    (void)cTraits; (void)iTraits;
    // MMGC_STATIC_ASSERT(sizeof(::flash::geom::ColorTransformClass::EmptySlotsStruct_ColorTransformClass) >= 0);
    MMGC_STATIC_ASSERT(offsetof(::flash::geom::ColorTransform, m_slots_ColorTransform) == s_slotsOffsetColorTransform);
    MMGC_STATIC_ASSERT(offsetof(::flash::geom::ColorTransform, m_slots_ColorTransform) <= 0xFFFF);
    MMGC_STATIC_ASSERT(sizeof(::flash::geom::ColorTransform) <= 0xFFFF);
    AvmAssert(getSlotOffset(iTraits, 5) == (offsetof(::flash::geom::ColorTransform, m_slots_ColorTransform) + offsetof(_flash_geom_ColorTransformSlots, m_alphaMultiplier)));
    AvmAssert(getSlotOffset(iTraits, 6) == (offsetof(::flash::geom::ColorTransform, m_slots_ColorTransform) + offsetof(_flash_geom_ColorTransformSlots, m_alphaOffset)));
    AvmAssert(getSlotOffset(iTraits, 7) == (offsetof(::flash::geom::ColorTransform, m_slots_ColorTransform) + offsetof(_flash_geom_ColorTransformSlots, m_blueMultiplier)));
    AvmAssert(getSlotOffset(iTraits, 8) == (offsetof(::flash::geom::ColorTransform, m_slots_ColorTransform) + offsetof(_flash_geom_ColorTransformSlots, m_blueOffset)));
    AvmAssert(getSlotOffset(iTraits, 9) == (offsetof(::flash::geom::ColorTransform, m_slots_ColorTransform) + offsetof(_flash_geom_ColorTransformSlots, m_greenMultiplier)));
    AvmAssert(getSlotOffset(iTraits, 10) == (offsetof(::flash::geom::ColorTransform, m_slots_ColorTransform) + offsetof(_flash_geom_ColorTransformSlots, m_greenOffset)));
    AvmAssert(getSlotOffset(iTraits, 11) == (offsetof(::flash::geom::ColorTransform, m_slots_ColorTransform) + offsetof(_flash_geom_ColorTransformSlots, m_redMultiplier)));
    AvmAssert(getSlotOffset(iTraits, 12) == (offsetof(::flash::geom::ColorTransform, m_slots_ColorTransform) + offsetof(_flash_geom_ColorTransformSlots, m_redOffset)));
}
#endif // DEBUG

AVMTHUNK_NATIVE_CLASS_GLUE(ColorTransformClass, ::flash::geom::ColorTransformClass, SlotOffsetsAndAsserts::doColorTransformClassAsserts)

AVMTHUNK_BEGIN_NATIVE_TABLES(auto_classes)

    AVMTHUNK_BEGIN_NATIVE_METHODS(auto_classes)
        AVMTHUNK_NATIVE_METHOD(flash_geom_ColorTransform_color_get, ::flash::geom::ColorTransform::get_color)
        AVMTHUNK_NATIVE_METHOD(flash_geom_ColorTransform_color_set, ::flash::geom::ColorTransform::set_color)
    AVMTHUNK_END_NATIVE_METHODS()

    AVMTHUNK_BEGIN_NATIVE_CLASSES(auto_classes)
        AVMTHUNK_NATIVE_CLASS(abcclass_flash_geom_ColorTransform, ColorTransformClass, ::flash::geom::ColorTransformClass, SlotOffsetsAndAsserts::s_slotsOffsetColorTransformClass, ::flash::geom::ColorTransform, SlotOffsetsAndAsserts::s_slotsOffsetColorTransform)
    AVMTHUNK_END_NATIVE_CLASSES()

AVMTHUNK_END_NATIVE_TABLES()

AVMTHUNK_DEFINE_NATIVE_INITIALIZER(auto_classes)

/* abc */
const uint8_t auto_classes_abc_data[645] = {
  16,   0,  46,   0,   2,   0,   0,   2,   0,   0,   0,   0,   0,   0, 240,  63,
  29,   0,  25, 102, 108,  97, 115, 104,  46, 103, 101, 111, 109,  58,  67, 111,
 108, 111, 114,  84, 114,  97, 110, 115, 102, 111, 114, 109,   6,  78, 117, 109,
  98, 101, 114,   5, 116, 114,  97,  99, 101,  12,  77, 121,  67, 108,  97, 115,
 115,  46,  97, 115,  36,  49,  10, 102, 108,  97, 115, 104,  46, 103, 101, 111,
 109,   6,  79,  98, 106, 101,  99, 116,   4, 108, 111, 108, 111,   3, 105, 110,
 116,   4, 118, 111, 105, 100,  15,  97, 108, 112, 104,  97,  77, 117, 108, 116,
 105, 112, 108, 105, 101, 114,  11,  97, 108, 112, 104,  97,  79, 102, 102, 115,
 101, 116,  14,  98, 108, 117, 101,  77, 117, 108, 116, 105, 112, 108, 105, 101,
 114,  10,  98, 108, 117, 101,  79, 102, 102, 115, 101, 116,  15, 103, 114, 101,
 101, 110,  77, 117, 108, 116, 105, 112, 108, 105, 101, 114,  11, 103, 114, 101,
 101, 110,  79, 102, 102, 115, 101, 116,  13, 114, 101, 100,  77, 117, 108, 116,
 105, 112, 108, 105, 101, 114,   9, 114, 101, 100,  79, 102, 102, 115, 101, 116,
   4, 116, 101, 115, 116,   5,  99, 111, 108, 111, 114,  14,  67, 111, 108, 111,
 114,  84, 114,  97, 110, 115, 102, 111, 114, 109,   6, 110,  97, 116, 105, 118,
 101,   3,  99, 108, 115,  34,  58,  58, 102, 108,  97, 115, 104,  58,  58, 103,
 101, 111, 109,  58,  58,  67, 111, 108, 111, 114,  84, 114,  97, 110, 115, 102,
 111, 114, 109,  67, 108,  97, 115, 115,   8, 105, 110, 115, 116,  97, 110,  99,
 101,  29,  58,  58, 102, 108,  97, 115, 104,  58,  58, 103, 101, 111, 109,  58,
  58,  67, 111, 108, 111, 114,  84, 114,  97, 110, 115, 102, 111, 114, 109,   7,
 109, 101, 116, 104, 111, 100, 115,   4,  97, 117, 116, 111,   9,   5,   2,  22,
   1,   5,   5,  22,   6,  23,   6,  24,   2,  26,   2,  26,   7,   3,   8,   1,
   2,   3,   4,   5,   6,   7,   8,   4,   2,   3,   4,   5,  18,   7,   2,   3,
   9,   4,   1,   7,   2,   9,   7,   2,  10,   7,   2,  11,   7,   2,  12,   7,
   2,  13,   7,   2,  14,   7,   2,  15,   7,   2,  16,   7,   2,  17,   7,   2,
  18,   7,   2,  19,   7,   2,  20,   7,   4,  21,   7,   2,   7,   9,   7,   2,
   7,   0,   0,   1,   0,   0,   0,   1,   0,   8,   0,   1,   1,   1,   1,   1,
   1,   1,   1,   1,   8,   8,   1,   6,   1,   6,   1,   6,   1,   6,   1,   3,
   1,   3,   1,   3,   1,   3,   0,   0,   1,   0,   0,   3,   1,  32,   1,   4,
   3,   1,  32,   0,   0,   1,   0,   1,  22,   3,  23,  25,  27,  24,  26,  28,
   1,  15,  16,   9,   6,   0,   2,  11,   5,   0,   0,   1,   0,   6,   0,   0,
   1,   0,   7,   0,   0,   1,   0,   8,   0,   0,   1,   0,   9,   0,   0,   1,
   0,  10,   0,   0,   1,   0,  11,   0,   0,   1,   0,  12,   0,   0,   1,   0,
  13,   1,   0,   3,  14,   2,   0,   4,  14,   3,   0,   5,   1,   0,   2,   6,
   1,  15,  68,   1,   0,   1,   0,   0,   0,   5,   0,   1,   1,   1,   2,   3,
 208,  48,  71,   0,   0,   1,   1,   1,   3,   4,   3, 208,  48,  71,   0,   0,
   2,   1,  10,   4,   5,  15, 208,  48, 208,  73,   0,  36,   0, 117,  99,   9,
  98,   9, 117, 213,  71,   0,   0,   3,   2,   1,   4,   5,  11, 208,  48,  93,
   2,  44,   8,  70,   2,   1,  41,  71,   0,   0,   6,   2,   1,   1,   3,  19,
 208,  48, 101,   0,  93,  16, 102,  16,  48,  93,  17, 102,  17,  88,   0,  29,
 104,  15,  71,   0,   0};

} }

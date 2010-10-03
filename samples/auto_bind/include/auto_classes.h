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

#define AVMTHUNK_VERSION 5

namespace flash { namespace geom {
    class ColorTransform; //flash.geom::ColorTransform
    class ColorTransformClass; //flash.geom::ColorTransform$
} }

namespace avmplus { namespace NativeID {

extern const uint32_t auto_classes_abc_class_count;
extern const uint32_t auto_classes_abc_script_count;
extern const uint32_t auto_classes_abc_method_count;
extern const uint32_t auto_classes_abc_length;
extern const uint8_t auto_classes_abc_data[];
AVMTHUNK_DECLARE_NATIVE_INITIALIZER(auto_classes)

/* classes */
const uint32_t abcclass_flash_geom_ColorTransform = 0;

/* methods */
const uint32_t flash_geom_ColorTransform_color_get = 4;
const uint32_t flash_geom_ColorTransform_color_set = 5;

extern AvmBox flash_geom_ColorTransform_color_get_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
extern AvmBox flash_geom_ColorTransform_color_set_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);

#ifdef VMCFG_INDIRECT_NATIVE_THUNKS

extern AvmBox auto_classes_i2a_o_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define flash_geom_ColorTransform_color_get_thunk  auto_classes_i2a_o_thunk

extern AvmBox auto_classes_v2a_oi_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define flash_geom_ColorTransform_color_set_thunk  auto_classes_v2a_oi_thunk

#endif // VMCFG_INDIRECT_NATIVE_THUNKS

class SlotOffsetsAndAsserts;
// flash.geom::ColorTransform$
//-----------------------------------------------------------
class _flash_geom_ColorTransformClassSlots
{
    friend class SlotOffsetsAndAsserts;
public:
private:
};
#define DECLARE_SLOTS_ColorTransformClass \
    private: \
        friend class avmplus::NativeID::SlotOffsetsAndAsserts; \
        typedef avmplus::NativeID::_flash_geom_ColorTransformClassSlots EmptySlotsStruct_ColorTransformClass
//-----------------------------------------------------------

// flash.geom::ColorTransform
//-----------------------------------------------------------
class _flash_geom_ColorTransformSlots
{
    friend class SlotOffsetsAndAsserts;
public:
    REALLY_INLINE double get_alphaMultiplier() const { return m_alphaMultiplier; }
    void set_alphaMultiplier(double newVal);
    REALLY_INLINE double get_alphaOffset() const { return m_alphaOffset; }
    void set_alphaOffset(double newVal);
    REALLY_INLINE double get_blueMultiplier() const { return m_blueMultiplier; }
    void set_blueMultiplier(double newVal);
    REALLY_INLINE double get_blueOffset() const { return m_blueOffset; }
    void set_blueOffset(double newVal);
    REALLY_INLINE double get_greenMultiplier() const { return m_greenMultiplier; }
    void set_greenMultiplier(double newVal);
    REALLY_INLINE double get_greenOffset() const { return m_greenOffset; }
    void set_greenOffset(double newVal);
    REALLY_INLINE double get_redMultiplier() const { return m_redMultiplier; }
    void set_redMultiplier(double newVal);
    REALLY_INLINE double get_redOffset() const { return m_redOffset; }
    void set_redOffset(double newVal);
private:
    double m_alphaMultiplier;
    double m_alphaOffset;
    double m_blueMultiplier;
    double m_blueOffset;
    double m_greenMultiplier;
    double m_greenOffset;
    double m_redMultiplier;
    double m_redOffset;
};
REALLY_INLINE void _flash_geom_ColorTransformSlots::set_alphaMultiplier(double newVal) { m_alphaMultiplier = newVal; }
REALLY_INLINE void _flash_geom_ColorTransformSlots::set_alphaOffset(double newVal) { m_alphaOffset = newVal; }
REALLY_INLINE void _flash_geom_ColorTransformSlots::set_blueMultiplier(double newVal) { m_blueMultiplier = newVal; }
REALLY_INLINE void _flash_geom_ColorTransformSlots::set_blueOffset(double newVal) { m_blueOffset = newVal; }
REALLY_INLINE void _flash_geom_ColorTransformSlots::set_greenMultiplier(double newVal) { m_greenMultiplier = newVal; }
REALLY_INLINE void _flash_geom_ColorTransformSlots::set_greenOffset(double newVal) { m_greenOffset = newVal; }
REALLY_INLINE void _flash_geom_ColorTransformSlots::set_redMultiplier(double newVal) { m_redMultiplier = newVal; }
REALLY_INLINE void _flash_geom_ColorTransformSlots::set_redOffset(double newVal) { m_redOffset = newVal; }
#define DECLARE_SLOTS_ColorTransform \
    private: \
        friend class avmplus::NativeID::SlotOffsetsAndAsserts; \
    protected: \
        REALLY_INLINE double get_alphaMultiplier() const { return m_slots_ColorTransform.get_alphaMultiplier(); } \
        REALLY_INLINE void set_alphaMultiplier(double newVal) { m_slots_ColorTransform.set_alphaMultiplier(newVal); } \
        REALLY_INLINE double get_alphaOffset() const { return m_slots_ColorTransform.get_alphaOffset(); } \
        REALLY_INLINE void set_alphaOffset(double newVal) { m_slots_ColorTransform.set_alphaOffset(newVal); } \
        REALLY_INLINE double get_blueMultiplier() const { return m_slots_ColorTransform.get_blueMultiplier(); } \
        REALLY_INLINE void set_blueMultiplier(double newVal) { m_slots_ColorTransform.set_blueMultiplier(newVal); } \
        REALLY_INLINE double get_blueOffset() const { return m_slots_ColorTransform.get_blueOffset(); } \
        REALLY_INLINE void set_blueOffset(double newVal) { m_slots_ColorTransform.set_blueOffset(newVal); } \
        REALLY_INLINE double get_greenMultiplier() const { return m_slots_ColorTransform.get_greenMultiplier(); } \
        REALLY_INLINE void set_greenMultiplier(double newVal) { m_slots_ColorTransform.set_greenMultiplier(newVal); } \
        REALLY_INLINE double get_greenOffset() const { return m_slots_ColorTransform.get_greenOffset(); } \
        REALLY_INLINE void set_greenOffset(double newVal) { m_slots_ColorTransform.set_greenOffset(newVal); } \
        REALLY_INLINE double get_redMultiplier() const { return m_slots_ColorTransform.get_redMultiplier(); } \
        REALLY_INLINE void set_redMultiplier(double newVal) { m_slots_ColorTransform.set_redMultiplier(newVal); } \
        REALLY_INLINE double get_redOffset() const { return m_slots_ColorTransform.get_redOffset(); } \
        REALLY_INLINE void set_redOffset(double newVal) { m_slots_ColorTransform.set_redOffset(newVal); } \
    private: \
        avmplus::NativeID::_flash_geom_ColorTransformSlots m_slots_ColorTransform
//-----------------------------------------------------------

} }

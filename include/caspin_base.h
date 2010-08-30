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

namespace avmplus {
    class ScriptObject; //caspin::System
}

namespace csp {
    class SystemClass; //caspin::System$
}

namespace avmplus { namespace NativeID {

extern const uint32_t caspin_base_abc_class_count;
extern const uint32_t caspin_base_abc_script_count;
extern const uint32_t caspin_base_abc_method_count;
extern const uint32_t caspin_base_abc_length;
extern const uint8_t caspin_base_abc_data[];
AVMTHUNK_DECLARE_NATIVE_INITIALIZER(caspin_base)

/* classes */
const uint32_t abcclass_caspin_System = 0;

/* methods */
const uint32_t caspin_System_trace = 4;

extern AvmBox caspin_System_trace_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);

#ifdef VMCFG_INDIRECT_NATIVE_THUNKS

extern AvmBox caspin_base_v2a_oo_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define caspin_System_trace_thunk  caspin_base_v2a_oo_thunk

#endif // VMCFG_INDIRECT_NATIVE_THUNKS

class SlotOffsetsAndAsserts;
// caspin::System$
//-----------------------------------------------------------
class _csp_SystemClassSlots
{
    friend class SlotOffsetsAndAsserts;
public:
private:
};
#define DECLARE_SLOTS_SystemClass \
    private: \
        friend class avmplus::NativeID::SlotOffsetsAndAsserts; \
        typedef avmplus::NativeID::_csp_SystemClassSlots EmptySlotsStruct_SystemClass
//-----------------------------------------------------------

} }

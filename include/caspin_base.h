#ifndef __CASPIN_BASE_GLUE_H__
#define __CASPIN_BASE_GLUE_H__

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

#ifndef _H_nativegen_header_caspin_base
#define _H_nativegen_header_caspin_base

namespace avmplus {
    class SystemObject; // caspin::System
}

namespace csp {
    class SystemClass; // caspin::System$
}

namespace avmplus { namespace NativeID {

extern const uint32_t caspin_base_abc_class_count;
extern const uint32_t caspin_base_abc_script_count;
extern const uint32_t caspin_base_abc_method_count;
extern const uint32_t caspin_base_abc_length;
extern const uint8_t caspin_base_abc_data[];
extern const char* const caspin_base_versioned_uris[];
AVMTHUNK_DECLARE_NATIVE_INITIALIZER(caspin_base)

/* classes */
const uint32_t abcclass_caspin_System = 0;

/* methods */
const uint32_t caspin_System_trace = 4;

extern avmplus::Atom caspin_System_trace_thunk(MethodEnv* env, uint32_t argc, Atom* argv);
class SlotOffsetsAndAsserts;
//-----------------------------------------------------------
// caspin::System$
//-----------------------------------------------------------
class csp_SystemClassSlots
{
    friend class SlotOffsetsAndAsserts;
    friend class csp::SystemClass;
};
#define DECLARE_SLOTS_SystemClass \
    public: \
        static avmplus::ClassClosure* FASTCALL createClassClosure(avmplus::VTable* cvtable); \
    public: \
        static avmplus::ScriptObject* FASTCALL createInstanceProc(avmplus::ClassClosure*); \
    public: \
        AvmThunk_DEBUG_ONLY( virtual avmplus::Atom construct(int argc, avmplus::Atom* argv); ) \
    private: \
        AvmThunk_DEBUG_ONLY( virtual void createInstance() { AvmAssert(0); } ) \
    public: \
        inline GCRef<avmplus::SystemObject> constructObject() \
        { \
            avmplus::Atom args[1] = { thisRef.reinterpretCast<avmplus::ScriptObject>()->atom() }; \
            avmplus::Atom const result = this->construct(0, args); \
            return GCRef<avmplus::SystemObject>((avmplus::SystemObject*)(avmplus::AvmCore::atomToScriptObject(result))); \
        } \
    public: \
        REALLY_INLINE bool isType(avmplus::Atom value) \
        { \
            return isTypeImpl(value); \
        } \
        REALLY_INLINE bool isType(GCRef<avmplus::ScriptObject> value) \
        { \
            return isTypeImpl(value->atom()); \
        } \
        REALLY_INLINE GCRef<avmplus::SystemObject> asType(avmplus::Atom value) \
        { \
            avmplus::Atom const result = asTypeImpl(value); \
            return GCRef<avmplus::SystemObject>((avmplus::SystemObject*)(avmplus::AvmCore::atomToScriptObject(result))); \
        } \
        REALLY_INLINE GCRef<avmplus::SystemObject> asType(GCRef<avmplus::ScriptObject> value) \
        { \
            avmplus::Atom const result = asTypeImpl(value->atom()); \
            return GCRef<avmplus::SystemObject>((avmplus::SystemObject*)(avmplus::AvmCore::atomToScriptObject(result))); \
        } \
        REALLY_INLINE GCRef<avmplus::SystemObject> coerceToType(avmplus::Atom value) \
        { \
            avmplus::Atom const result = coerceToTypeImpl(value); \
            return GCRef<avmplus::SystemObject>((avmplus::SystemObject*)(avmplus::AvmCore::atomToScriptObject(result))); \
        } \
        REALLY_INLINE GCRef<avmplus::SystemObject> coerceToType(GCRef<avmplus::ScriptObject> value) \
        { \
            avmplus::Atom const result = coerceToTypeImpl(value->atom()); \
            return GCRef<avmplus::SystemObject>((avmplus::SystemObject*)(avmplus::AvmCore::atomToScriptObject(result))); \
        } \
    private: \
        friend class avmplus::NativeID::SlotOffsetsAndAsserts; \

//-----------------------------------------------------------

//-----------------------------------------------------------
// caspin::System
//-----------------------------------------------------------
class avmplus_SystemObjectSlots
{
    friend class SlotOffsetsAndAsserts;
    friend class avmplus::SystemObject;
#define GC_TRIVIAL_TRACER_SystemObject
};
//-----------------------------------------------------------

} }
namespace avmplus {

class caspin_baseClassManifest : public avmplus::ClassManifestBase
{
    friend class avmplus::AvmCore;
    friend class avmplus::IntVectorClass;
    friend class avmplus::UIntVectorClass;
    // friend class avmplus::FloatVectorClass;
    friend class avmplus::DoubleVectorClass;
    friend class avmplus::ObjectVectorClass;
private:
    REALLY_INLINE caspin_baseClassManifest(avmplus::ScriptEnv* e) : ClassManifestBase(1, e) { }
    REALLY_INLINE static caspin_baseClassManifest* create(avmplus::ScriptEnv* e) { return new (MMgc::GC::GetGC(e), MMgc::kExact, sizeof(ClassClosure*)*0) caspin_baseClassManifest(e); }
public:
    REALLY_INLINE GCRef<csp::SystemClass> get_SystemClass() { return (csp::SystemClass*)(lazyInitClass(avmplus::NativeID::abcclass_caspin_System)); }
};
}
#endif // _H_nativegen_header_caspin_base

#endif // __CASPIN_BASE_GLUE_H__

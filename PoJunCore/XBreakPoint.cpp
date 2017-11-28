#include "stdafx.h"
#include "XBreakPoint.h"
#include "XInt3Tab.h"
#include "XHardwareBreak.h"


XBreakPoint* XBreakPoint::pThis = 0;
XBreakPoint::XBreakPoint()
{
}


XBreakPoint::~XBreakPoint()
{
}
 
XBreakPoint* XBreakPoint::pins()
{
    if (pThis == nullptr)
    {
        pThis = new XBreakPoint;
    }

    return pThis;
}

BP_STATUS XBreakPoint::break_point(EXCEPTION_RECORD* er, tagDebugInfo& debug_info)
{ 
    //此时触发的EIP已经指向了CC所在地址之后的一位偏移，修正EIP
    --debug_info.context.Eip;

    if (XInt3Tab::pins()->is_my_cc(debug_info.process, debug_info.context.Eip))
    {
        return BP_CC; 
    }
    else if (XInt3Tab::pins()->is_p_single_step(debug_info.process, debug_info.context.Eip))
    {
        return BP_P_SINGLE_STEP;
    } 
    else if (XInt3Tab::pins()->is_start_opcode(*(DWORD*)&er->ExceptionAddress))
    {  
        return BP_OEP;
    } 

    return BP_NULL;
}

BP_STATUS XBreakPoint::reduction_oep(HANDLE handle)
{
    bool status = XInt3Tab::pins()->reduction_oep(handle);
    return status ? BP_OK : BP_NULL;
}

BP_STATUS XBreakPoint::reduction_cc(HANDLE handle, DWORD address, bool status)
{
    status = XInt3Tab::pins()->reduction_cc(handle, address, status);
    return status ? BP_OK : BP_NULL;
}

DWORD XBreakPoint::get_reduction_single_step()
{
    return XInt3Tab::pins()->get_reduction_single_step();
}

BP_STATUS XBreakPoint::set_reduction_single_step(CONTEXT& context)
{
    bool status = XInt3Tab::pins()->set_reduction_single_step(context);
    return status ? BP_OK : BP_NULL;
}
  
BP_STATUS XBreakPoint::insert_cc(HANDLE handle, DWORD address)
{
    bool status = XInt3Tab::pins()->insert_cc(handle, address); 
    return status ? BP_OK : BP_NULL;
}

BP_STATUS XBreakPoint::set_cc_status(int inedx, bool status)
{ 
    bool ret = XInt3Tab::pins()->set_cc_status(inedx, status);
    return ret ? BP_OK : BP_NULL;
}

BP_STATUS XBreakPoint::get_cc_table(std::map<DWORD, CC_BREAK_POINT>& out_map)
{
    bool status = XInt3Tab::pins()->get_cc_table(out_map);
    return status ? BP_OK : BP_NULL;
}

BP_STATUS XBreakPoint::delete_cc_inedx(int inedx)
{
    bool status = XInt3Tab::pins()->delete_cc_inedx(inedx);
    return status ? BP_OK : BP_NULL;
}

BP_STATUS XBreakPoint::insert_p_single_step(HANDLE handle, DWORD address)
{
    bool status = XInt3Tab::pins()->insert_p_single_step(handle, address);
    return status ? BP_OK : BP_NULL; 
}

BP_STATUS XBreakPoint::insert_hard_break(std::vector<XString>& vt_command, CONTEXT& context)
{
    bool status = XHardwareBreak::pins()->insert(vt_command, context);
    return status ? BP_OK : BP_NULL;
}

BP_STATUS XBreakPoint::get_hard_ware_break_tab(std::vector<HARD_DWARE_BREAK_POINT>& out_map)
{
    bool status = XHardwareBreak::pins()->get_hard_dware_break_table(out_map);
    return status ? BP_OK : BP_NULL;
}
 
BP_STATUS XBreakPoint::delete_hard_ware_break_inedx(CONTEXT& context, int inedx)
{
    bool status = XHardwareBreak::pins()->delete_hard_ware_break_inedx(context, inedx);
    return status ? BP_OK : BP_NULL;
}
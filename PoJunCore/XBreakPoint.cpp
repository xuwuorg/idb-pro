#include "stdafx.h"
#include "XBreakPoint.h"
#include "XInt3Tab.h"


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
    if (XInt3Tab::pins()->is_my_cc(debug_info.process, --debug_info.context.Eip))
    {
        return BP_CC; 
    }
    else if (XInt3Tab::pins()->is_single_step(debug_info.process, debug_info.context.Eip))
    {
        return BP_SINGLE_STEP;
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

BP_STATUS XBreakPoint::insert_single_step(HANDLE handle, DWORD address)
{
    bool status = XInt3Tab::pins()->insert_single_step(handle, address);
    return status ? BP_OK : BP_NULL; 
}


#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>

#include <cal_instruction_ta.h>
enum
{
	OP_BR = 0, /* branch */
	OP_ADD,	   /* add  */
	OP_LD,	   /* load */
	OP_ST,	   /* store */
	OP_JSR,	   /* jump register */
	OP_AND,	   /* bitwise and */
	OP_LDR,	   /* load register */
	OP_STR,	   /* store register */
	OP_RTI,	   /* unused */
	OP_NOT,	   /* bitwise not */
	OP_LDI,	   /* load indirect */
	OP_STI,	   /* store indirect */
	OP_JMP,	   /* jump */
	OP_RES,	   /* reserved (unused) */
	OP_LEA,	   /* load effective address */
	OP_TRAP	   /* execute trap */
};
TEE_Result TA_CreateEntryPoint(void)
{
	return TEE_SUCCESS;
}

void TA_DestroyEntryPoint(void)
{
}

TEE_Result TA_OpenSessionEntryPoint(uint32_t param_types,
		TEE_Param __maybe_unused params[4],
		void __maybe_unused **sess_ctx)
{
	uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE);
	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;

	(void)&params;
	(void)&sess_ctx;

	return TEE_SUCCESS;
}

void TA_CloseSessionEntryPoint(void __maybe_unused *sess_ctx)
{
	(void)&sess_ctx;
}

static TEE_Result calc_instruction(uint32_t param_types,
	TEE_Param params[4])
{
	uint32_t exp_param_types =
				TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INOUT,
						TEE_PARAM_TYPE_VALUE_INOUT,
						TEE_PARAM_TYPE_NONE,
						TEE_PARAM_TYPE_NONE);

	DMSG("has been called");
	uint16_t ins = params[0].value.a;
	int op = ins >> 12;
	uint16_t judge = (ins >> 5)& 0x1;
    	uint16_t a1 = (ins >> 6) & 0x7;
	uint16_t a0 = ins & 0x7;
	uint16_t a2 ;
	IMSG("p:  %hu from NW", op);
	IMSG("ins:  %hu from NW", ins);
	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;
	switch (op)
	{
		case OP_ADD:
			IMSG("ADD");
			
			if(judge == 0)
			{
				a2 = a1 + a0;			
			}
			else
			{
				a0 = ins & 0x1F;
				a2 = a1 + a0;		
			}
			IMSG("num:%u",a0);
			IMSG("num:%u",a1);
			IMSG("num:%u",a2);
			params[0].value.a = a2;
			break;
		case OP_AND:
			IMSG("Bitwise AND");	
			if(judge == 0)
			{
				a2 = a1 & a0;			
			}
			else
			{
				a0 = ins & 0x1F;
				a2 = a1 & a2;		
			}
			IMSG("num:%u",a0);			
			IMSG("num:%u",a1);
			params[0].value.a = a2;
			break;
		
	}
	IMSG("Getting time.");
	/*
	 * The TEE_GenerateRandom function is a part of TEE Internal Core API,
	 * which generates random data
	 *
	 * Parameters:
	 * @ randomBuffer : Reference to generated random data
	 * @ randomBufferLen : Byte length of requested random data
	 */
	//TEE_Time t;
	//TEE_GetSystemTime(&t);
	//params[0].value.a = t.seconds;
	//params[0].value.b = t.millis;

	return TEE_SUCCESS;
}

TEE_Result TA_InvokeCommandEntryPoint(void __maybe_unused *sess_ctx,
			uint32_t cmd_id,
			uint32_t param_types, TEE_Param params[4])
{
	(void)&sess_ctx;
	IMSG("Got value: %u from NW", params[0].value.a);
	switch (cmd_id) {
	case TA_CALC_INSTRUCTION:
		return calc_instruction(param_types, params);
	default:
		return TEE_ERROR_BAD_PARAMETERS;
	}
}



#include <err.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

/* OP-TEE TEE client API (built by optee_client) */
#include <tee_client_api.h>

/* To the the UUID (found the the TA's h-file(s)) */
#include <cal_instruction_ta.h>

int main(int argc,char *argv[])
{
	TEEC_Result res;
	TEEC_Context ctx;
	TEEC_Session sess;
	TEEC_Operation op = { 0 };
	TEEC_UUID uuid = TA_CMD_CALC_INSTRUCTION_UUID;
	uint8_t random_uuid[16] = { 0 };
	uint32_t err_origin;
	int i;

	/* Initialize a context connecting us to the TEE */
	res = TEEC_InitializeContext(NULL, &ctx);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_InitializeContext failed with code 0x%x", res);

	/*
	 * Open a session to the Random example TA, the TA will print "hello
	 * world!" in the log when the session is created.
	 */
	res = TEEC_OpenSession(&ctx, &sess, &uuid,
			       TEEC_LOGIN_PUBLIC, NULL, NULL, &err_origin);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_Opensession failed with code 0x%x origin 0x%x",
			res, err_origin);

	/*
	 * Execute a function in the TA by invoking it, in this case
	 * we're incrementing a number.
	 *
	 * The value of command ID part and how the parameters are
	 * interpreted is part of the interface provided by the TA.
	 */

	/* Clear the TEEC_Operation struct */
	memset(&op, 0, sizeof(op));

	/*
	 * Prepare the argument. Pass a value in the first parameter,
	 * the remaining three parameters are unused.
	 */
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INOUT,
					 TEEC_VALUE_INOUT, TEEC_NONE, TEEC_NONE);

	//printf("String: %s\n", argv[1]);
	int q = 1;
    	char *p;
	int num = 0;
	int cnt = 15;
    	
        for(p=*(argv+q); *p; p++)
	{
		int a = cnt;
		if(*p == '1'){
			int b = 1;	
			//printf("is one ");						
			for(a = cnt;a > 0;a--)
			{
				b = b * 2;
			}				
			num+=b;
		}        	
		else{
					
		}
		//printf("%c\n", *p);
		cnt = cnt-1;
	}
       	


	op.params[0].value.a = num;
	

	/*
	 * TA_EXAMPLE_RANDOM_GENERATE is the actual function in the TA to be
	 * called.
	 */
	printf("calculating... \n");
	res = TEEC_InvokeCommand(&sess, TA_CALC_INSTRUCTION,
				 &op, &err_origin);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_InvokeCommand failed",
			res, err_origin);
	
	printf("Result: %d\n", op.params[0].value.a);
	//printf(", millis: %d\n", op.params[0].value.b);

	/*
	 * We're done with the TA, close the session and
	 * destroy the context.
	 *
	 * The TA will print "Goodbye!" in the log when the
	 * session is closed.
	 */

	TEEC_CloseSession(&sess);

	TEEC_FinalizeContext(&ctx);

	return 0;
}

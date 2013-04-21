#include <globus/globus_ftp_client.h>

int dy_gftp_init()
{
	int ret;
	ret = globus_module_activate(GLOBUS_FTP_CLIENT_MODULE);
	if (ret != 0) {
		return ret;
	}
	return 0;
}

int dy_gftp_read()
{
	return 0;
}

int dy_gftp_write()
{
	return 0;
}

int dy_gftp_cleanup()
{
	int ret;
	ret = globus_module_deactivate(GLOBUS_FTP_CLIENT_MODULE);
	if (ret != 0) {
		return ret;
	}
	return 0;
}

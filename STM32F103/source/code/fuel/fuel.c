#include "config.h"

config_t g_data;
/*判断密码是否正确*/
int is_valid_pwd(char *left, char *right) {
	int *a, *b;
	short *c, *d;

	a = (int *) left;
	b = (int *) right;
	c = (short *) (left + 4);
	d = (short *) (right + 4);

	if ((*a == *b) && (*c == *d)) {
		return TRUE;
	}

	return FALSE;
}

/*转换密码，从字符串到数组*/
void get_password(char *buf, char *pwd) {
	char *from = pwd;
	char *to = buf;
	int i = PWD_LEN;

	while (0 != *from && i > 0) {
		*to = *from - 48;
		to++;
		from++;
		i--;
	}
	return;
}

/*转换密码，从数组到字符串*/
void set_password(char *buf, char *pwd) {
	char *from = buf;
	char *to = pwd;
	int i = PWD_LEN;

	while (0 != *from && i > 0) {
		*to = *from + 48;
		to++;
		from++;
		i--;
	}
	return;
}

/*去除\n 字符*/
void del_one(char *buf) {
	int len = 0;
	len = strlen(buf);
	if ((len > 0) && (buf[len - 1] == '\n')) {
		buf[len - 1] = 0;
	}
	return;
}


int get_scanf(char *buf, char *name, char *value) {
	char *findit = NULL;
	int len = 0;
	int i = 0;

	findit = buf;
	len = strlen(buf);

	for (i = 0; i < len; ++i) {
		if (*findit == '=')
			goto next;

		findit++;
	}
	return ERR;

	next: strncpy(name, buf, LINE);
	name[i] = 0;
	strncpy(value, findit + 1, LINE);
	return OK;
}


void read_param_config(FIL *fp) {
	char buf[BUF_SIZ] = { 0 };
	char name[LINE + 1] = { 0 };
	char value[LINE + 1] = { 0 };
	int ret = OK;

	while (f_gets(buf, BUF_SIZ, fp)) {
		del_one(buf); /*去除\n 字符*/

		ret = get_scanf(buf, name, value);
		if (ERR == ret) /*格式不对，跳过*/
		{
			continue;
		}

		/*now , start load config context*/

		if (!strncmp("name", name, LINE)) {
			strncpy(g_data.name, value, LINE);
			continue;
		}

		if (!strncmp("password", name, LINE)) {
			get_password(g_data.pwd, value);
			continue;
		}

		if (!strncmp("capacity", name, LINE)) {
			g_data.capacity = (unsigned int) atoi(value);
			continue;
		}

		if (!strncmp("shape", name, LINE)) {
			g_data.shape = (unsigned short) atoi(value);
			continue;
		}

		if (!strncmp("height", name, LINE)) {
			g_data.u.cube.height = (unsigned short) atoi(value);
			continue;
		}

		if (!strncmp("length", name, LINE)) {
			g_data.u.cube.length = (unsigned short) atoi(value);
			continue;
		}

		if (!strncmp("width", name, LINE)) {
			g_data.u.cube.width = (unsigned short) atoi(value);
			continue;
		}

		if (!strncmp("radius", name, LINE)) {
			g_data.u.cylinder.radius = (unsigned short) atoi(value);
			continue;
		}

		if (!strncmp("len", name, LINE)) {
			g_data.u.cylinder.len = (unsigned short) atoi(value);
			continue;
		}
	}

	return;
}


int check_pwd_get_action(FIL *fp, int *action) {
	char buf[BUF_SIZ] = { 0 };
	char name[LINE + 1] = { 0 };
	char value[LINE + 1] = { 0 };
	int ret = OK;

	/*get password*/
	res = f_read(fp, buf, 18, &br);
	if (!is_valid_pwd(g_data.pwd, buf)) {
		printf("password is wrong, please check it !!\n");
		return ERR;
	}

	/*get acticon*/
	f_gets(buf, BUF_SIZ, fp);
	del_one(buf); /*去除\n 字符*/

	ret = get_scanf(buf, name, value);
	if (ERR == ret) /*格式不对，返回*/
	{
		printf("action line wrong, please check it !!\n");
		return ERR;
	}

	if (!strncmp("ACTION", name, LINE)) {
		*action = atoi(value);
		return OK;
	}

	printf("action line wrong, please check it !!\n");
	return ERR;
}

/*save config to data flash*/
int save_config(void) {
	FIL confp;
	char buf[BUF_SIZ] = { 0 };
	char password[PWD_LEN + 1] = { 0 };
	int str_pnt = 0;
	int len = 0;

	set_password(g_data.pwd, password);

	len = sprintf(buf + str_pnt, "#version %s\n", VERSION);
	str_pnt += len;

	len = sprintf(buf + str_pnt, "name=%s\n", g_data.name);
	str_pnt += len;

	len = sprintf(buf + str_pnt, "password=%s\n", password);
	str_pnt += len;

	len = sprintf(buf + str_pnt, "capacity=%d\n", g_data.capacity);
	str_pnt += len;

	len = sprintf(buf + str_pnt, "shape=%d\n", g_data.shape);
	str_pnt += len;


	if (CYLINDER == g_data.shape) {
		len = sprintf(buf + str_pnt, "radius=%d\n", g_data.u.cylinder.radius);
		str_pnt += len;
		len = sprintf(buf + str_pnt, "len=%d\n", g_data.u.cylinder.len);
		str_pnt += len;
	} else /*CUBE */
	{
		len = sprintf(buf + str_pnt, "length=%d\n", g_data.u.cube.length);
		str_pnt += len;
		len = sprintf(buf + str_pnt, "width=%d\n", g_data.u.cube.width);
		str_pnt += len;
		len = sprintf(buf + str_pnt, "height=%d\n", g_data.u.cube.height);
		str_pnt += len;
	}

	f_chdrive(0);
	printf("\r\n 写入初始配置");
	res = f_open(&confp, DEFAULT_CONFIG_FILE, FA_CREATE_ALWAYS | FA_WRITE);
	if (res) {
		printf("\r\n 1打开DataFlash 失败 ");
		return;
	}
	printf("\r\n 1打开DataFlash 成功 ");
	res = f_lseek(&confp, confp.fsize);
	res = f_write(&confp, buf, str_pnt, &bw);
	//res = f_write(&fdst, "device.txt", 4, &bw);
	f_close(&fdst);

	return OK;
}


int load_config(void) {
	char buf[BUF_SIZ] = { 0 };
	char name[LINE + 1] = { 0 };
	char value[LINE + 1] = { 0 };
	int ret = OK;

	f_mount(0, &fs[0]);
	f_mount(1, &fs[1]);
	f_chdrive(0);
	printf("\r\n 读系统配置");
	res = f_open(&fdst, DEFAULT_CONFIG_FILE, FA_OPEN_EXISTING | FA_READ);
	if (res) {
		printf("\r\n 1打开DataFlash 失败 ");
		return;
	}
	printf("\r\n 1打开DataFlash 成功 ");

	/*first , version check, now do nothing : ) */
	f_gets(buf, BUF_SIZ, &fdst);

	/*now , start load config context*/
	read_param_config(&fdst);

	f_close(&fdst);

	return ret;
}


int load_config_from_sd(int *action) {
	int ret = OK;
	char sdfn[40];
	strcpy(sdfn, "1:/machine/");
	strcat(sdfn, machine);
	strcat(sdfn, "/sd.txt");
	f_mount(0, &fs[0]);
	f_mount(1, &fs[1]);
	f_chdrive(1);
	printf("\r\n 读SD配置");
	res = f_open(&fdst, sdfn, FA_OPEN_EXISTING | FA_WRITE | FA_READ);

	ret = check_pwd_get_action(&fdst, action); /*检查密码，获取action*/

	if (ERR == ret) {
		printf("load_config_from_sd wrong, please check it !!\n");
		f_close (&fdst);
		return ERR;
	}

	/*如果action是更新配置，就读取配置到内存,并且保存配置到data flash*/
	if (W_CONFIG == *action) {
		read_param_config(&fdst);
		save_config(); /*save config to data flash*/
	}

	f_close (&fdst);
	return OK;
}


void write_file_tmp(void) {
	char buf[BUF_SIZ] = { 0 };
	char password[PWD_LEN + 1] = { 0 };
	int str_pnt = 0;
	int len = 0;

	len = sprintf(buf + str_pnt, "#version 1.0.1\n");
	str_pnt += len;

	len = sprintf(buf + str_pnt, "name=machine1\n");
	str_pnt += len;

	f_mount(0, &fs[0]);
	f_mount(1, &fs[1]);
	f_chdrive(0);
	printf("\r\n 写入初始配置");
	res = f_open(&fdst, DEFAULT_CONFIG_FILE, FA_CREATE_ALWAYS | FA_WRITE);
	if (res) {
		printf("\r\n 1打开DataFlash 失败 ");
		return;
	}
	printf("\r\n 1打开DataFlash 成功 ");
	res = f_lseek(&fdst, fdst.fsize);
	res = f_write(&fdst, buf, str_pnt, &bw);
	//res = f_write(&fdst, "device.txt", 4, &bw);
	f_close(&fdst);

}



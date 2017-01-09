#include "public.h"

typedef struct FS_HEAD {
	u8 fs_type; //file system type
	u8 fs_version; //for update migrate
	u8 sector_size; //default size is 1, for future use.
	u8 fs_file_table_address; //can added copy to table2 for safety
	char logo[8];
	u16 fs_file_start_address;
	u16 future;
}FS_HEAD;
const static struct FS_HEAD fs_head = {
	0x01, 0x01, 0x01,   0x10, "AISTLFS", 0x0100, 0x0000
};

#define MAX_FILE_ADDRESS 1000

const u8 file_xor_sum_offset = 0xE5;
u16 file_tail;

u8 max_file_number = 0;
static u8 calc_xor_sum(FILE_INFO f) {
	u8 xor_sum = file_xor_sum_offset;
	u8 *p = (u8 *) &f;
	for (int i = 0; (p + i) < &f.xor_sum; i++) {
		xor_sum ^= *(p + i);
	}
	return xor_sum;
}
#define index_to_address(index) (fs_head.fs_file_table_address + index * sizeof(FILE_INFO))
static int is_valid_file(FILE_INFO f) {
	if (f.start_address < fs_head.fs_file_start_address)
	return 0;
	return calc_xor_sum(f) == f.xor_sum;
}
static inline void _write_eeproms(u8 *buf, u16 address, u16 len)
{
	for (int i = 0; i < len; i++)
	{
		//EEPROM.write(address + i, buf[i]);
	}
}
static inline void _set_eeproms( u16 address, u8 value, u16 len)
{
	printf("set ee: %d %d %d\n", value, address, len);
	for (int i = 0; i < len; i++)
	{
		//EEPROM.write(address + i, value);
	}
}
static inline void _read_eeproms(u8 *buf, u16 address, u16 len)
{
	for (int i = 0; i < len; i++)
	{
		//buf[i] = EEPROM.read(address + i);
	}
}

int get_file_by_index(u8 index, FILE_INFO *out)
{
	FILE_INFO TMP;
	//EEPROM.get(index_to_address(index), TMP);
	if (is_valid_file(TMP))
	{
		memcpy(out, &TMP, sizeof(FILE_INFO));
		return  FILE_SUCCESS;
	}
	return FILE_NOT_EXIST;
}

static int find_file_index(const char name[], FILE_INFO *fout, u8 *out_index) {
	int len = max_file_number;
	for (int i = 0; i < len; i++)
	{
		FILE_INFO TMP;
		int ret = get_file_by_index(i, &TMP);
		if (ret == FILE_SUCCESS)
		{
			int t = TMP.start_address + TMP.size;
			if (file_tail < t)
			{
				file_tail = t;
			}
			if (0 == strcmp(TMP.name, name))
			{
				memcpy(fout , &TMP, sizeof(FILE_INFO));
				*out_index = i;
				return FILE_SUCCESS;
			}

		}
		else //for get empty file
		{
			*out_index = i;
			return FILE_NOT_EXIST;
		}
	}
	return FILE_REACH_MAX;
}


int init_file_system(void) {
	FS_HEAD tmp;
	//EEPROM.get(0, tmp);
	if (strcmp(tmp.logo, fs_head.logo) != 0)
	{
		//EEPROM.put(0, fs_head);
		_set_eeproms(index_to_address(0), 0,  sizeof(FILE_INFO)*max_file_number);
	}
	max_file_number = (fs_head.fs_file_start_address - fs_head.fs_file_table_address) / sizeof(FILE_INFO);
	file_tail= fs_head.fs_file_start_address;
	return FILE_SUCCESS;
}
int get_file(const char name[], FILE_INFO *fout)
{
	u8 index;
	return find_file_index(name, fout, &index);
}
int read_file(FILE_INFO FI, u8 buf[]) {
	_read_eeproms(buf, FI.start_address, FI.size);
	return FILE_SUCCESS;
}

int write_file(const char name[], u8 buf[], u8 len) {
	FILE_INFO TMP;
	u8 index;
	int ret = find_file_index(name, &TMP, &index);
	if (ret == FILE_SUCCESS)
	{
		return FILE_ALREADY_EXIST;
	}
	else if (ret == FILE_REACH_MAX)
	{
		return DISK_NOT_ENOUGH;
	}
	TMP.start_address = file_tail;
	TMP.size = len;
	memset(TMP.name, 0, 8);
	memcpy(TMP.name, name, strlen(name));
	TMP.xor_sum = calc_xor_sum(TMP);
	//EEPROM.put(index_to_address(index), TMP);
	_write_eeproms(buf, TMP.start_address, TMP.size);
	file_tail += TMP.size;
	return FILE_SUCCESS;
}
int delete_file(const char name[]) {
	FILE_INFO TMP;
	u8 index;
	int ret = find_file_index(name, &TMP, &index);
	if (ret == FILE_SUCCESS)
	{
		_set_eeproms(  index_to_address(index), 0, sizeof(FILE_INFO));
	}
	return ret;
}

void test_fs()
{
	u8 buf[40] = "hello file.";
	const char fn[] = "prime";
	int c = write_file(fn, buf, 40);
	printf("write '%s' state:%d\n", fn, c);
	FILE_INFO f;
	if (FILE_SUCCESS == get_file(fn, &f))
	{
		u8 *fbuf = malloc(f.size);
		read_file(f, fbuf);
		printf(get_flash_str(MSG_VM_MANAGER_LOADED_CODE_FMT), f.name, f.size);
	}
	for (int i = 0; i < index_to_address(2); i++)
	{
		//int c = EEPROM.read(i);
		printf(" %d\t\t % d\t %c\n", i, c, c);
	}
	for (int i = fs_head.fs_file_start_address; i < fs_head.fs_file_start_address + 20; i++)
	{
		//int c = EEPROM.read(i);
		printf(" %d\t\t % d\t %c\n", i, c, c);
	}
}

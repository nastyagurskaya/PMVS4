#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <string.h>
#include <errno.h>

#define NAME_LENGTH 255
static int *file_offset_end;
static const char **filename;
static int *file_size;
static int file_count = 0;
#define STORE_FILE "/home/nastya/Desktop/PMVS4/all_file"
#define BUF_FILE "/home/nastya/Desktop/PMVS4/buffer_file"
struct file_info {
	char file_name[NAME_LENGTH];
	int file_size;
	int file_offset;
};
static int path_index(const char* path)
{
	int i  = 0;
	for (i = 0; i < file_count; i++) {
		if (strcmp(file_name[i], path)==0) {
			return i;
		}
	}
	return -1;
}
static int getattr_callback(const char *path, struct stat *stbuf) {
  memset(stbuf, 0, sizeof(struct stat));

  if (strcmp(path, "/") == 0) {
    stbuf->st_mode = S_IFDIR | 0755;
    stbuf->st_nlink = 2;
    return 0;
  }
  else {
	int index = path_index(path);
	if(index == -1){
		return -ENOENT;
	}
	stbuf->st_mode = S_IFREG | 0777;
	stbuf->st_nlink = 1;
	int start = index == 0 ? 0 : file_offset_end[index-1];
		int size = file_offset_end[index]-start;
		printf("%d\n", size);
	stbuf->st_size = file_size[index];
	return 0;
  }

  return -ENOENT;
}

static int readdir_callback(const char *path, void *buf, fuse_fill_dir_t filler,
    off_t offset, struct fuse_file_info *fi) {
  (void) offset;
  (void) fi;

  filler(buf, ".", NULL, 0);
  filler(buf, "..", NULL, 0);

	for (i = 0; i < file_count; i++) {
		if(strlen(file_name[i])!= 0) {
  		filler(buf, filename[i]+1, NULL, 0);
		}
	}

  return 0;
}

static int open_callback(const char *path, struct fuse_file_info *fi) {
	int index = path_index(path);
	if (index == -1)
		return -ENOENT;
  	return 0;
}

static int read_callback(const char *path, char *buf, size_t size, off_t offset,
    struct fuse_file_info *fi) {

  if (strcmp(path, filepath) == 0) {
    size_t len = strlen(filecontent);
    if (offset >= len) {
      return 0;
    }

    if (offset + size > len) {
      memcpy(buf, filecontent + offset, len - offset);
      return len - offset;
    }

    memcpy(buf, filecontent + offset, size);
    return size;
  }

  return -ENOENT;
}

static struct fuse_operations fuse_example_operations = {
  .getattr = getattr_callback,
  .open = open_callback,
  .read = read_callback,
  .readdir = readdir_callback,
};

int main(int argc, char *argv[])
{
  return fuse_main(argc, argv, &fuse_example_operations, NULL);
}


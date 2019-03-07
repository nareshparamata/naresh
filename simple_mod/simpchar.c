
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/poll.h>
#include <linux/ioport.h>
#include <linux/errno.h>
#include <linux/cdev.h>

MODULE_AUTHOR("DEPIK Systems");
MODULE_LICENSE("GPL");

static int mydev_open(struct inode *inode, struct file *file);
static int mydev_close(struct inode *inode, struct file *file);
static ssize_t mydev_read(struct file *file, char *buf, size_t count,
                          loff_t *offset);
static ssize_t mydev_write(struct file *file, const char *buf, size_t count,
                           loff_t *offset);


#define  MAX_BUF_SIZE  1024

typedef struct 
{
  char buf[1024];
  int noOfChars;
  struct cdev mycdev;
}mydev_t;

mydev_t mydev;

#define MYDEV_MAJOR_NUM   42
#define MYDEV_MINOR_NUM   0
#define MYDEV_NO_DEVS     10
#define MYDEV_NAME        "mychardev"

static struct file_operations mydev_ops =
{
  .read    = mydev_read,
  .write   = mydev_write,
  .open    = mydev_open,
  .release = mydev_close
};

/*******************************************************************************
* Name:init_module
* Description:
*******************************************************************************/
int mydev_init(void)
{
  int   res   = 0;
  dev_t devno = 0;

  devno = MKDEV(MYDEV_MAJOR_NUM, MYDEV_MINOR_NUM);
  res = register_chrdev_region(devno, MYDEV_NO_DEVS, MYDEV_NAME);

  if(res<0)
  {
    printk("Register char dev region Error %d\n",res);
    return res;
  }
  else
    printk("Register char dev region success %d\n",res);

  cdev_init(&mydev.mycdev, &mydev_ops);
  mydev.mycdev.owner = THIS_MODULE;
  res = cdev_add(&mydev.mycdev, devno, 1);
  if(res)
    goto fail_exit;
  printk("cdev added successully %d\n",res);
  mydev.noOfChars=0;
  return 0;

fail_exit:
  unregister_chrdev_region(devno, MYDEV_NO_DEVS);
  return res;
}

/*******************************************************************************
* Name:cleanup_module
* Description:
*******************************************************************************/
void mydev_cleanup(void)
{
  dev_t devno = MKDEV(MYDEV_MAJOR_NUM, MYDEV_MINOR_NUM);

  cdev_del(&mydev.mycdev);

  unregister_chrdev_region(devno, MYDEV_NO_DEVS);

  printk(KERN_ALERT "char dev unloaded successfully \n");
}

/*******************************************************************************
* Name:mydev_open
* Description:
*******************************************************************************/
static int mydev_open(struct inode *inode, struct file * file)
{
  try_module_get(THIS_MODULE);
  file->private_data = &mydev;
  if (file->f_mode & FMODE_READ)
    printk("<1>" "open for read\n");
  if (file->f_mode & FMODE_WRITE)
    printk("<1>" "opened for write\n");
  
  return 0;	
}


/*******************************************************************************
* Name:mydev_close
* Description:
*******************************************************************************/
static int mydev_close(struct inode *inode, struct file * file)
{
  module_put(THIS_MODULE);
  printk("<1>" "device closed\n");
  return 0;
}


/*******************************************************************************
* Name:mydev_read
* Description:
*******************************************************************************/
static ssize_t mydev_read(struct file *file, char * buf,
                          size_t count,  loff_t * offset)
{
  mydev_t *tdev = file->private_data;
  if (count>tdev->noOfChars) 
    count = tdev->noOfChars;
  copy_to_user(buf,tdev->buf,count);
	
  printk("<1>" "read called \n");
  return (ssize_t)count;
}

/*******************************************************************************
* Name:mydev_write
* Description:
*******************************************************************************/
static int mydev_write(struct file * file, const char * buf,
                       size_t count,loff_t * offset)
{
  mydev_t *tdev = file->private_data;
  if (count > MAX_BUF_SIZE) 
    count = MAX_BUF_SIZE;
  copy_from_user(tdev->buf,buf,count);
  tdev->noOfChars=count;
    printk("<1>" "write called \n");
  return count;
}

module_init(mydev_init);
module_exit(mydev_cleanup);

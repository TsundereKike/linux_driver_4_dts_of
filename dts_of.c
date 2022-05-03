#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/of.h>
#include <linux/of_address.h>
/*模块入口*/
static int __init dts_of_init(void)
{
    int ret = 0;
    int i = 0;
    const char *sta = NULL;
    u32 def_value = 0;
    u32 elemsize = 0;
    u32 *brival = NULL;
    struct device_node *bl_nod = NULL;
    struct property * bl_pr = NULL;
    /*1：找到backlight节点，路径是：/backlight*/
    bl_nod = of_find_node_by_path("/backlight");
    if(bl_nod==NULL){/*失败*/
        ret = -EINVAL;
        goto fail_find_node;
    }
    /*2:获取属性*/
    bl_pr = of_find_property(bl_nod,"compatible",NULL);
    if(bl_pr==NULL){/*失败*/
        ret = -EINVAL;
        goto fail_find_prop;
    }
    else/*成功*/
    {
        printk("compatible = %s\r\n",(char*)bl_pr->value);
    }
    ret = of_property_read_string(bl_nod,"status",&sta);
    if(ret<0){
        goto fail_reads;
    }
    else
    {
        printk("status = %s\r\n",sta);
    }
    /*读取数字属性*/
    ret = of_property_read_u32(bl_nod,"default-brightness-level",&def_value);
    if(ret<0){
        goto fail_read32;
    }
    else
    {
        printk("default-brightness-level = %d\r\n",def_value);
    }
    /*获取数组类型的属性*/
    elemsize = of_property_count_elems_of_size(bl_nod,"brightness-levels", sizeof(u32));
    if(elemsize<0){
        ret = -EINVAL;
        goto fail_readele;
    }
    else
    {
        printk("brightness-levels = %d\r\n",elemsize);
    }
    /*申请内存*/
    brival = kmalloc(elemsize * sizeof(u32),GFP_KERNEL);
    if(!brival){
        ret = -EINVAL;
        goto fail_mem;
    }
    /*获取数组元素*/
    ret = of_property_read_u32_array(bl_nod,"brightness-levels",brival, elemsize);
    if(ret<0){
        goto fail_read32array;
    }
    else
    {
        for(i=0;i<elemsize;i++)
        {
            printk("brightness-levels[%d] = %d\r\n",i,brival[i]);            
        }
    }
    kfree(brival);

    return 0;

fail_read32array:
    kfree(brival);/*释放内存*/
fail_mem:

fail_readele:

fail_read32:

fail_reads:

fail_find_prop:

fail_find_node:
    return ret;
}
/*模块出口*/
static void __exit dts_of_exit(void)
{
    
}

/*注册模块入口及出口*/
module_init(dts_of_init);
module_exit(dts_of_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("tanminghang");


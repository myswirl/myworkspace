/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// 文件名
 * Author:			// 作者
 * Date:			// 日期
 * Description:		// 模块描述
 * Version:			// 版本信息
 * History:			// 历史修改记录
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>
#include <libglade-2.0/glade/glade.h>                       // 包含libglade函数库
#include "LogFile.h"
#include "XMS_ApiTest_Sub.h"

GtkWidget *text_view;

/***********************************************************
* Function:       // main
* Description:    // 入口函数
***********************************************************/
int main( int argc, char *argv[] )
{
	gtk_init( &argc, &argv );                               // 初始化GTK+库
	GladeXML	*ui;                                        // 声明GladeXML类型变量

	ui = glade_xml_new( "xms_apitest.glade", NULL, NULL );  // 创建GladeXML对象
	GtkWidget	*window;                                    // 声明GtkWidget类型变量
	window = glade_xml_get_widget( ui, "main_dialog" );
	// 从GladeXML对象获得GtkWidget界面构件
	GtkWidget *label;
	label = glade_xml_get_widget( ui, "label1" );
	gtk_label_set_label( GTK_LABEL( label ), "Hello World!" );

	text_view = glade_xml_get_widget (ui, "textview1");
	
	// 修改界面构件的属性
	gtk_widget_show_all( window );                          // 显示window内的所有构件
	glade_xml_signal_autoconnect( ui );                     // 连接GladeXML对象所有已定义信号

	InitSystem( );                                          //初始化系统

	gtk_main( );                                            // 开始GTK+主循环
	return 0;
}
/************************************** The End Of File **************************************/

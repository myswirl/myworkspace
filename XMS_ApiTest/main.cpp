/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// �ļ���
 * Author:			// ����
 * Date:			// ����
 * Description:		// ģ������
 * Version:			// �汾��Ϣ
 * History:			// ��ʷ�޸ļ�¼
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>
#include <libglade-2.0/glade/glade.h>                       // ����libglade������
#include "LogFile.h"
#include "XMS_ApiTest_Sub.h"

GtkWidget *text_view;

/***********************************************************
* Function:       // main
* Description:    // ��ں���
***********************************************************/
int main( int argc, char *argv[] )
{
	gtk_init( &argc, &argv );                               // ��ʼ��GTK+��
	GladeXML	*ui;                                        // ����GladeXML���ͱ���

	ui = glade_xml_new( "xms_apitest.glade", NULL, NULL );  // ����GladeXML����
	GtkWidget	*window;                                    // ����GtkWidget���ͱ���
	window = glade_xml_get_widget( ui, "main_dialog" );
	// ��GladeXML������GtkWidget���湹��
	GtkWidget *label;
	label = glade_xml_get_widget( ui, "label1" );
	gtk_label_set_label( GTK_LABEL( label ), "Hello World!" );

	text_view = glade_xml_get_widget (ui, "textview1");
	
	// �޸Ľ��湹��������
	gtk_widget_show_all( window );                          // ��ʾwindow�ڵ����й���
	glade_xml_signal_autoconnect( ui );                     // ����GladeXML���������Ѷ����ź�

	InitSystem( );                                          //��ʼ��ϵͳ

	gtk_main( );                                            // ��ʼGTK+��ѭ��
	return 0;
}
/************************************** The End Of File **************************************/

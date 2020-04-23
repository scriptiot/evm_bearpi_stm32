#include "evm_module.h"
#include "./lcd/lcd.h"

//LCD()
static evm_val_t evm_module_lcd(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc != 0 ) EVM_ARG_LENGTH_ERR
	LCD_Init();
	return EVM_VAL_UNDEFINED;
}

//LCD().on()
static evm_val_t evm_module_lcd_on(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc != 0 ) EVM_ARG_LENGTH_ERR

	LCD_DisplayOn();
	return EVM_VAL_UNDEFINED;
}

//LCD.off()
static evm_val_t evm_module_lcd_off(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc != 0 ) EVM_ARG_LENGTH_ERR

	LCD_DisplayOff();
	return EVM_VAL_UNDEFINED;
}

//LCD.set_pixel(x, y)
static evm_val_t evm_module_lcd_set_pixel(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc != 2 ) EVM_ARG_LENGTH_ERR
	if( !evm_is_number(v) ) EVM_ARG_TYPE_ERR;
	if( !evm_is_number(v + 1) ) EVM_ARG_TYPE_ERR;
	int x = evm_2_integer(v);
	int y = evm_2_integer(v + 1);
	LCD_Draw_Point(x, y);
	return EVM_VAL_UNDEFINED;
}
//color = LCD.get_pixel(x, y)
//static evm_val_t evm_module_lcd_get_pixel(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
//{
//	if( argc < 2 ) EVM_ARG_LENGTH_ERR
//	if( !evm_is_number(v) ) EVM_ARG_TYPE_ERR;
//	if( !evm_is_number(v + 1) ) EVM_ARG_TYPE_ERR;
//	int x = evm_2_integer(v);
//	int y = evm_2_integer(v + 1);
//	return evm_mk_number(ILI9341_GetPointPixel(x, y));
//}

//LCD.draw_line(x1, y1, x2, y2)
static evm_val_t evm_module_lcd_draw_line(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc > 3 ){
		if( !evm_is_number(v) ) EVM_ARG_TYPE_ERR;
		if( !evm_is_number(v + 1) ) EVM_ARG_TYPE_ERR;
		if( !evm_is_number(v + 2) ) EVM_ARG_TYPE_ERR;
		if( !evm_is_number(v + 3) ) EVM_ARG_TYPE_ERR;
		int x1 = evm_2_integer(v);
		int y1 = evm_2_integer(v + 1);
		int x2 = evm_2_integer(v + 2);
		int y2 = evm_2_integer(v + 3);
		LCD_DrawLine(x1, y1, x2, y2);
	}
	return EVM_VAL_UNDEFINED;
}

//LCD.draw_rect(x1, y1, x2, y2)
static evm_val_t evm_module_lcd_draw_rect(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc > 3 ){
		if( !evm_is_number(v) ) EVM_ARG_TYPE_ERR;
		if( !evm_is_number(v + 1) ) EVM_ARG_TYPE_ERR;
		if( !evm_is_number(v + 2) ) EVM_ARG_TYPE_ERR;
		if( !evm_is_number(v + 3) ) EVM_ARG_TYPE_ERR;
		int x1 = evm_2_integer(v);
		int y1 = evm_2_integer(v + 1);
		int x2 = evm_2_integer(v + 2);
		int y2 = evm_2_integer(v + 3);
		LCD_DrawRectangle(x1, y1, x2, y2);
	}
	return EVM_VAL_UNDEFINED;
}

//LCD.fill_rect(x1, y1, x2, y2,color)
static evm_val_t evm_module_lcd_fill_rect(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc > 4 ){
		if( !evm_is_number(v) ) EVM_ARG_TYPE_ERR;
		if( !evm_is_number(v + 1) ) EVM_ARG_TYPE_ERR;
		if( !evm_is_number(v + 2) ) EVM_ARG_TYPE_ERR;
		if( !evm_is_number(v + 3) ) EVM_ARG_TYPE_ERR;
		if( !evm_is_number(v + 4) ) EVM_ARG_TYPE_ERR;

		int x1 = evm_2_integer(v);
		int y1 = evm_2_integer(v + 1);
		int x2 = evm_2_integer(v + 2);
		int y2 = evm_2_integer(v + 3);
		int color = evm_2_integer(v + 4);

		LCD_Fill(x1, y1, x2, y2, color);
	}
	return EVM_VAL_UNDEFINED;
}

//LCD.draw_circle(x, y, radius)
static evm_val_t evm_module_lcd_draw_circle(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc > 2 ){
		if( !evm_is_number(v) ) EVM_ARG_TYPE_ERR;
		if( !evm_is_number(v + 1) ) EVM_ARG_TYPE_ERR;
		if( !evm_is_number(v + 2) ) EVM_ARG_TYPE_ERR;
		int x1 = evm_2_integer(v);
		int y1 = evm_2_integer(v + 1);
		int r = evm_2_integer(v + 2);
		LCD_Draw_Circle(x1, y1, r);
	}
	return EVM_VAL_UNDEFINED;
}

////LCD.fill_circle(x, y, colour)
//static evm_val_t evm_module_lcd_fill_circle(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
//{
//	if( argc > 3 ){
//		if( !evm_is_number(v) ) EVM_ARG_TYPE_ERR;
//		if( !evm_is_number(v + 1) ) EVM_ARG_TYPE_ERR;
//		if( !evm_is_number(v + 2) ) EVM_ARG_TYPE_ERR;
//		int x1 = evm_2_integer(v);
//		int y1 = evm_2_integer(v + 1);
//		int r = evm_2_integer(v + 2);
//		ILI9341_DrawCircle(x1, y1, r, 1);
//	}
//	return EVM_VAL_UNDEFINED;
//}

//LCD.draw_text(x, y,w,h,size,text)
static evm_val_t evm_module_lcd_draw_text(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc > 5 ){
		if( !evm_is_number(v) ) EVM_ARG_TYPE_ERR;
		if( !evm_is_number(v + 1) ) EVM_ARG_TYPE_ERR;
		if( !evm_is_string(v + 2) ) EVM_ARG_TYPE_ERR;
		if( !evm_is_number(v + 3) ) EVM_ARG_TYPE_ERR;
		if( !evm_is_string(v + 4) ) EVM_ARG_TYPE_ERR;
		if( !evm_is_number(v + 5) ) EVM_ARG_TYPE_ERR;
		
		int x1 = evm_2_integer(v);
		int y1 = evm_2_integer(v + 1);
		int width = evm_2_integer(v+2);
		int heigh = evm_2_integer(v + 3);
		int size = evm_2_integer(v+4);
		char * text = (char*)evm_2_string(v+5);
		LCD_ShowString(x1, y1, width,heigh,size,text);
	}
	return EVM_VAL_UNDEFINED;
}

//LCD.set_text_color(rgb)
static evm_val_t evm_module_lcd_set_text_color(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc > 0 ){
		if( !evm_is_number(v) ) EVM_ARG_TYPE_ERR;
		int color = evm_2_integer(v);
		LCD_Set_Color(color);
	}
	return EVM_VAL_UNDEFINED;
}

//LCD.set_background_color(rgb)
static evm_val_t evm_module_lcd_set_background_color(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc > 0 ){
		if( !evm_is_number(v) ) EVM_ARG_TYPE_ERR;
		int color = evm_2_integer(v);
		LCD_Clear(color);
	}
	return EVM_VAL_UNDEFINED;
}

evm_val_t evm_class_lcd(evm_t * e){
	evm_builtin_t class_lcd[] = {
		{"on", evm_mk_native( (intptr_t)evm_module_lcd_on )},
		{"off", evm_mk_native( (intptr_t)evm_module_lcd_off )},
		{"set_pixel", evm_mk_native( (intptr_t)evm_module_lcd_set_pixel )},
//		{"get_pixel", evm_mk_native( (intptr_t)evm_module_lcd_get_pixel )},
		{"draw_line", evm_mk_native( (intptr_t)evm_module_lcd_draw_line )},
		{"draw_rect", evm_mk_native( (intptr_t)evm_module_lcd_draw_rect )},
		{"fill_rect", evm_mk_native( (intptr_t)evm_module_lcd_fill_rect )},
		{"draw_circle", evm_mk_native( (intptr_t)evm_module_lcd_draw_circle )},
//		{"fill_circle", evm_mk_native( (intptr_t)evm_module_lcd_fill_circle )},
		{"draw_text", evm_mk_native( (intptr_t)evm_module_lcd_draw_text )},
		{"set_text_color", evm_mk_native( (intptr_t)evm_module_lcd_set_text_color )},
		{"set_background_color", evm_mk_native( (intptr_t)evm_module_lcd_set_background_color )},
		
		{"WHITE",evm_mk_number((uint32_t)0xFFFF)},
		{"BLACK",evm_mk_number((uint32_t)0x0000)},
		{"BLUE",evm_mk_number((uint32_t)0x001F)},
		{"BRED",evm_mk_number((uint32_t)0XF81F)},
		{"GRED",evm_mk_number((uint32_t)0XFFE0)},
		{"GBLUE",evm_mk_number((uint32_t)0X07FF)},
		{"RED",evm_mk_number((uint32_t)0xF800)},
		{"MAGENTA",evm_mk_number((uint32_t)0xF81F)},
		{"GREEN",evm_mk_number((uint32_t)0x07E0)},
		{"CYAN",evm_mk_number((uint32_t)0x7FFF)},
		{"YELLOW",evm_mk_number((uint32_t)0xFFE0)},
		{"BROWN",evm_mk_number((uint32_t)0XBC40)},
		{"BRRED",evm_mk_number((uint32_t)0XFC07)},
		{"GRAY",evm_mk_number((uint32_t)0X8430)},
		{"DARKBLUE",evm_mk_number((uint32_t)0X01CF)},
		{"LIGHTBLUE",evm_mk_number((uint32_t)0X7D7C)},
		{"GRAYBLUE",evm_mk_number((uint32_t)0X5458)},
		{"LIGHTGREEN",evm_mk_number((uint32_t)0X841F)},
		{"LGRAY",evm_mk_number((uint32_t)0XC618)},
		{"LGRAYBLUE",evm_mk_number((uint32_t)0XA651)},
		{"LBBLUE",evm_mk_number((uint32_t)0X2B12)},
				
		{NULL, NULL}
	};
	return *evm_class_create(e, (evm_native_fn)evm_module_lcd, class_lcd, NULL);
}

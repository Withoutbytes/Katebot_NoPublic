#include "../main.h"
#include "input_system.hpp"

cs_virtual_table	cs_vt_inputsystem;
uint32_t			cs_nv_m_dwButton;

uintptr_t input_system::GetPointer(void)
{
	return cs_vt_inputsystem.address();
}

bool input_system::IsButtonDown(ButtonCode_t button) {
	uint32_t v;

	v = mem->Read<uint32_t>(cs_vt_inputsystem.address() + (((button >> 0x5) * 0x4) + cs_nv_m_dwButton));
	return (v >> (button & 0x1F)) & 1;
}

void input_system::Init() {
	cs_interface_reg inputsystem;

	inputsystem = interfaces::find(xorstr("inputsystem.dll").crypt_get());
	cs_vt_inputsystem = inputsystem.find(xorstr("InputSystemVersion").crypt_get());

	cs_nv_m_dwButton = mem->Read<uint32_t>(cs_vt_inputsystem.function(0xF) + 0x21D);
}

ButtonCode_t input_system::vktobc(int vk) {
	std::unordered_map<std::string, ButtonCode_t> const table = {
			{xorstr("0").crypt_get(), KEY_0}, {xorstr("1").crypt_get(), KEY_1}, {xorstr("2").crypt_get(), KEY_2}, {xorstr("3").crypt_get(), KEY_3},
			{xorstr("4").crypt_get(), KEY_4}, {xorstr("5").crypt_get(), KEY_5}, {xorstr("6").crypt_get(), KEY_6}, {xorstr("7").crypt_get(), KEY_7},
			{xorstr("8").crypt_get(), KEY_8}, {xorstr("9").crypt_get(), KEY_9}, { xorstr("A").crypt_get(), KEY_A },{ xorstr("B").crypt_get(), KEY_B },
			{ xorstr("C").crypt_get(), KEY_C }, { xorstr("D").crypt_get(), KEY_D }, { xorstr("E").crypt_get(), KEY_E }, { xorstr("F").crypt_get(), KEY_F },
			{ xorstr("G").crypt_get(), KEY_G }, { xorstr("H").crypt_get(), KEY_H }, { xorstr("I").crypt_get(), KEY_I }, { xorstr("J").crypt_get(), KEY_J },
			{ xorstr("K").crypt_get(), KEY_K }, { xorstr("L").crypt_get(), KEY_L }, { xorstr("M").crypt_get(), KEY_M }, { xorstr("N").crypt_get(), KEY_N },
			{ xorstr("O").crypt_get(), KEY_O }, { xorstr("P").crypt_get(), KEY_P }, { xorstr("Q").crypt_get(), KEY_Q }, { xorstr("R").crypt_get(), KEY_R },
			{ xorstr("S").crypt_get(), KEY_S }, { xorstr("T").crypt_get(), KEY_T }, { xorstr("U").crypt_get(), KEY_U }, { xorstr("V").crypt_get(), KEY_V },
			{ xorstr("W").crypt_get(), KEY_W }, { xorstr("X").crypt_get(), KEY_X }, { xorstr("Y").crypt_get(), KEY_Y }, { xorstr("Z").crypt_get(), KEY_Z },
	};

	if ((vk >= '0' && vk <= '9') || (vk >= 'A' && vk <= 'Z'))
		if (auto it = table.find(std::to_string(vk)); it != table.end())
			return it->second;

	switch (vk)
	{
		c_vk_bc(VK_LBUTTON, MOUSE_LEFT);
		c_vk_bc(VK_RBUTTON, MOUSE_RIGHT);
		c_vk_bc(VK_MBUTTON, MOUSE_MIDDLE);
		c_vk_bc(VK_XBUTTON1, MOUSE_4);
		c_vk_bc(VK_XBUTTON2, MOUSE_5);
		c_vk_bc(VK_BACK, KEY_BACKSPACE);
		c_vk_bc(VK_TAB, KEY_TAB);
		c_vk_bc(VK_RETURN, KEY_ENTER);
		c_vk_bc(VK_SHIFT, KEY_LSHIFT);
		c_vk_bc(VK_LSHIFT, KEY_LSHIFT);
		c_vk_bc(VK_RSHIFT, KEY_RSHIFT);
		c_vk_bc(VK_CONTROL, KEY_LCONTROL);
		c_vk_bc(VK_LCONTROL, KEY_LCONTROL);
		c_vk_bc(VK_RCONTROL, KEY_RCONTROL);
		c_vk_bc(VK_MENU, KEY_LALT);
		c_vk_bc(VK_LMENU, KEY_LALT);
		c_vk_bc(VK_RMENU, KEY_RALT);
		c_vk_bc(VK_INSERT, KEY_INSERT);
		c_vk_bc(VK_DELETE, KEY_DELETE);
		c_vk_bc(VK_HOME, KEY_HOME);
		c_vk_bc(VK_NUMPAD0, KEY_PAD_0);
		c_vk_bc(VK_NUMPAD1, KEY_PAD_1);
		c_vk_bc(VK_NUMPAD2, KEY_PAD_2);
		c_vk_bc(VK_NUMPAD3, KEY_PAD_3);
		c_vk_bc(VK_NUMPAD4, KEY_PAD_4);
		c_vk_bc(VK_NUMPAD5, KEY_PAD_5);
		c_vk_bc(VK_NUMPAD6, KEY_PAD_6);
		c_vk_bc(VK_NUMPAD7, KEY_PAD_7);
		c_vk_bc(VK_NUMPAD8, KEY_PAD_8);
		c_vk_bc(VK_NUMPAD9, KEY_PAD_9);
		c_vk_bc(VK_F1, KEY_F1);
		c_vk_bc(VK_F2, KEY_F2);
		c_vk_bc(VK_F3, KEY_F3);
		c_vk_bc(VK_F4, KEY_F4);
		c_vk_bc(VK_F5, KEY_F5);
		c_vk_bc(VK_F6, KEY_F6);
		c_vk_bc(VK_F7, KEY_F7);
		c_vk_bc(VK_F8, KEY_F8);
		c_vk_bc(VK_F9, KEY_F9);
		c_vk_bc(VK_F10, KEY_F10);
		c_vk_bc(VK_F11, KEY_F11);
		c_vk_bc(VK_F12, KEY_F12);
	}

	return BUTTON_CODE_NONE;
}
#ifndef INPUT_EXTRAS_H
#define INPUT_EXTRAS_H

// Translators
static inline RSButtons TranslateGLFW(int i)
{
	switch(i)
	{
		case GLFW_KEY_ESCAPE:		return RS_KEY_ESC;
		case GLFW_KEY_F1:			return RS_KEY_F1;
		case GLFW_KEY_F2:			return RS_KEY_F2;
		case GLFW_KEY_F3:			return RS_KEY_F3;
		case GLFW_KEY_F4:			return RS_KEY_F4;
		case GLFW_KEY_F5:			return RS_KEY_F5;
		case GLFW_KEY_F6:			return RS_KEY_F6;
		case GLFW_KEY_F7:			return RS_KEY_F7;
		case GLFW_KEY_F8:			return RS_KEY_F8;
		case GLFW_KEY_F9:			return RS_KEY_F9;
		case GLFW_KEY_F10:			return RS_KEY_F10;
		case GLFW_KEY_F11:			return RS_KEY_F11;
		case GLFW_KEY_F12:			return RS_KEY_F12;
		case GLFW_KEY_F13:			return RS_KEY_F13;
		case GLFW_KEY_F14:			return RS_KEY_F14;
		case GLFW_KEY_F15:			return RS_KEY_F15;
		case GLFW_KEY_LEFT:			return RS_KEY_LEFT;
		case GLFW_KEY_DOWN:			return RS_KEY_DOWN;
		case GLFW_KEY_RIGHT:		return RS_KEY_RIGHT;
		case GLFW_KEY_UP:			return RS_KEY_UP;
		case GLFW_KEY_LEFT_SHIFT:	return RS_KEY_SHIFT_1;
		case GLFW_KEY_RIGHT_SHIFT:	return RS_KEY_SHIFT_1;
		case GLFW_KEY_LEFT_CONTROL:	return RS_KEY_CTRL_1;
		case GLFW_KEY_RIGHT_CONTROL:return RS_KEY_CTRL_2;
		case GLFW_KEY_LEFT_ALT:		return RS_KEY_ALT_1;
		case GLFW_KEY_RIGHT_ALT:	return RS_KEY_ALT_2;
		case GLFW_KEY_LSUPER:		return RS_KEY_SUPER_1;
		case GLFW_KEY_RSUPER:		return RS_KEY_SUPER_2;
		case GLFW_KEY_TAB:			return RS_KEY_TAB;
		case GLFW_KEY_ENTER:		return RS_KEY_ENTER;
		case GLFW_KEY_BACKSPACE:	return RS_KEY_BACKSPACE;
		case GLFW_KEY_INSERT:		return RS_KEY_INSERT;
		case GLFW_KEY_DEL:			return RS_KEY_DELETE;
		case GLFW_KEY_PAGEUP:		return RS_KEY_PGUP;
		case GLFW_KEY_PAGEDOWN:		return RS_KEY_PGDOWN;
		case GLFW_KEY_HOME:			return RS_KEY_HOME;
		case GLFW_KEY_END:			return RS_KEY_END;
		case GLFW_KEY_KP_0:			return RS_KEY_KP_0;
		case GLFW_KEY_KP_1:			return RS_KEY_KP_1;
		case GLFW_KEY_KP_2:			return RS_KEY_KP_2;
		case GLFW_KEY_KP_3:			return RS_KEY_KP_3;
		case GLFW_KEY_KP_4:			return RS_KEY_KP_4;
		case GLFW_KEY_KP_5:			return RS_KEY_KP_5;
		case GLFW_KEY_KP_6:			return RS_KEY_KP_6;
		case GLFW_KEY_KP_7:			return RS_KEY_KP_7;
		case GLFW_KEY_KP_8:			return RS_KEY_KP_8;
		case GLFW_KEY_KP_9:			return RS_KEY_KP_9;
		case GLFW_KEY_KP_DIVIDE:	return RS_KEY_KP_SLASH;
		case GLFW_KEY_KP_MULTIPLY:	return RS_KEY_KP_ASTERISK;
		case GLFW_KEY_KP_SUBTRACT:	return RS_KEY_KP_HYPHEN;
		case GLFW_KEY_KP_ADD:		return RS_KEY_KP_PLUS;
		case GLFW_KEY_KP_DECIMAL:	return RS_KEY_KP_PERIOD;
		case GLFW_KEY_KP_ENTER:		return RS_KEY_KP_ENTER;

		case 'A':	return RS_KEY_A;
		case 'B':	return RS_KEY_B;
		case 'C':	return RS_KEY_C;
		case 'D':	return RS_KEY_D;
		case 'E':	return RS_KEY_E;
		case 'F':	return RS_KEY_F;
		case 'G':	return RS_KEY_G;
		case 'H':	return RS_KEY_H;
		case 'I':	return RS_KEY_I;
		case 'J':	return RS_KEY_J;
		case 'K':	return RS_KEY_K;
		case 'L':	return RS_KEY_L;
		case 'M':	return RS_KEY_M;
		case 'N':	return RS_KEY_N;
		case 'O':	return RS_KEY_O;
		case 'P':	return RS_KEY_P;
		case 'Q':	return RS_KEY_Q;
		case 'R':	return RS_KEY_R;
		case 'S':	return RS_KEY_S;
		case 'T':	return RS_KEY_T;
		case 'U':	return RS_KEY_U;
		case 'V':	return RS_KEY_V;
		case 'W':	return RS_KEY_W;
		case 'X':	return RS_KEY_X;
		case 'Y':	return RS_KEY_Y;
		case 'Z':	return RS_KEY_Z;
		case '1':	return RS_KEY_1;
		case '2':	return RS_KEY_2;
		case '3':	return RS_KEY_3;
		case '4':	return RS_KEY_4;
		case '5':	return RS_KEY_5;
		case '6':	return RS_KEY_6;
		case '7':	return RS_KEY_7;
		case '8':	return RS_KEY_8;
		case '9':	return RS_KEY_9;
		case '0':	return RS_KEY_0;
		case '`':	return RS_KEY_TILDE;
		case '-':	return RS_KEY_HYPHEN;
		case '=':	return RS_KEY_EQUALS;
		case '[':	return RS_KEY_LEFT_SQ_BRACKET;
		case ']':	return RS_KEY_RIGHT_SQ_BRACKET;
		case '\\':	return RS_KEY_BACK_SLASH;
		case ';':	return RS_KEY_SEMICOLON;
		case '\'':	return RS_KEY_APOSTROPHE;
		case ',':	return RS_KEY_COMMA;
		case '.':	return RS_KEY_PERIOD;
		case '/':	return RS_KEY_FORWARD_SLASH;
		case ' ':	return RS_KEY_SPACE;

		case GLFW_MOUSE_BUTTON_1:	return RS_MOUSE_1;
		case GLFW_MOUSE_BUTTON_2:	return RS_MOUSE_2;
		case GLFW_MOUSE_BUTTON_3:	return RS_MOUSE_3;
		case GLFW_MOUSE_BUTTON_4:	return RS_MOUSE_4;
		case GLFW_MOUSE_BUTTON_5:	return RS_MOUSE_5;
		case GLFW_MOUSE_BUTTON_6:	return RS_MOUSE_6;
		case GLFW_MOUSE_BUTTON_7:	return RS_MOUSE_7;
		case GLFW_MOUSE_BUTTON_8:	return RS_MOUSE_8;

		default:
			break;
	}
	
	// Fallback
	return RS_KEY_INVALID;
}

#endif // INPUT_EXTRAS_H

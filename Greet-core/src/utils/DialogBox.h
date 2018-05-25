#if 0
#pragma once

namespace Greet { namespace DialogBox {

	enum class Style {
		Info,
		Warning,
		Error,
		Question
	};

	enum class Buttons {
		OK,
		OKCancel,
		YesNo
	};

	enum class Selection {
		OK,
		Cancel,
		Yes,
		No,
		None
	};

	const Style DEFAULT_STYLE = Style::Info;
	const Buttons DEFAULT_BUTTONS = Buttons::OK;

	Selection Show(const char *message, const char *title, Style style, Buttons buttons);

	inline Selection Show(const char *message, const char *title, Style style) {
		return Show(message, title, style, DEFAULT_BUTTONS);
	}

	inline Selection Show(const char *message, const char *title, Buttons buttons) {
		return Show(message, title, DEFAULT_STYLE, buttons);
	}

	inline Selection Show(const char *message, const char *title) {
		return Show(message, title, DEFAULT_STYLE, DEFAULT_BUTTONS);
	}

}}
#endif

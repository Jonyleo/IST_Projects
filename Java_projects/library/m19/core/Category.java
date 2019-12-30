package m19.core;

import java.io.Serializable;

enum Category implements Serializable {
	REFERENCE("Referência"), FICTION("Ficção"), SCITECH("Técnica e Científica");
	private String _str;

	Category(String str) {
		_str = str;
	}

	String getString() {
		return _str;
	}
}
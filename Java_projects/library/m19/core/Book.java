package m19.core;

class Book extends Work {
	private String _writer;
	private String _isbn;

	Book(int id, String title, int price, int total, String cat, String wri, String isbn) {
		super(id, title, price, total, cat);

		_writer = wri;
		_isbn =	isbn;
	}

	@Override
	protected boolean matchCreator(String searchTerm) {
		return _writer.toLowerCase().contains(searchTerm);
	}

	@Override
	protected String getSpecialEnding() {
		return _writer + " - " + _isbn;
	}

	@Override
	protected String getType() {
		return "Livro";
	}
}
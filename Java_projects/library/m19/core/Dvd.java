package m19.core;

class Dvd extends Work {
	private String _director;
	private String _igac;

	Dvd(int id, String title, int price, int total, String cat, String dir, String igac) {
		super(id, title, price, total, cat);

		_director = dir;
		_igac = igac;
	}

	@Override
	protected boolean matchCreator(String searchTerm) {
		return _director.toLowerCase().contains(searchTerm);
	}

	@Override
	protected String getSpecialEnding() {
		return _director + " - " + _igac;
	}

	@Override
	protected String getType() {
		return "DVD";
	}
}
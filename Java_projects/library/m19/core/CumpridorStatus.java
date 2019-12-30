package m19.core;

class CumpridorStatus extends AbstractStatus {
	private static CumpridorStatus _self;
	
	private CumpridorStatus() {
		super(5, "CUMPRIDOR");
	}

	static AbstractStatus get(){
		if(_self == null)
			_self = new CumpridorStatus();
		return _self;
	}

	@Override
	boolean canRequestWithPrice(int price) {
		return true;
	}

	@Override
	int getDeadline(int nCopies) {
		if(nCopies == 1)
			return 8;
		else if(nCopies <= 5)
			return 15;
		else 
			return 30;
	}
}
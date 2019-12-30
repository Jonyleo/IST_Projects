package m19.core;

class FaltosoStatus extends AbstractStatus {
	private static FaltosoStatus _self;
	
	private FaltosoStatus() {
		super(1, "FALTOSO");
	}

	static AbstractStatus get(){
		if(_self == null)
			_self = new FaltosoStatus();
		return _self;
	}
 
	@Override
	boolean canRequestWithPrice(int price) {
		return !(price > 25);
	}

	@Override
	int getDeadline(int nCopies) {
		return 2;
	}

}
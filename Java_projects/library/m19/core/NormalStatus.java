package m19.core;

class NormalStatus extends AbstractStatus {
	private static NormalStatus _self;
	
	private NormalStatus() {
		super(3, "NORMAL");
	}

	static AbstractStatus get(){
		if(_self == null)
			_self = new NormalStatus();
		return _self;
	}

	@Override
	boolean canRequestWithPrice(int price) {
		return !(price > 25);
	}

	@Override
	int getDeadline(int nCopies) {
		if(nCopies == 1)
			return 3;
		else if(nCopies <= 5)
			return 8;
		else 
			return 15;
	}

}
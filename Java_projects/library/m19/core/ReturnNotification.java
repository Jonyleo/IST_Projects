package m19.core;

class ReturnNotification extends AbstractNotification {
	ReturnNotification(Work work) {
		super("ENTREGA: ", work);
	}

	@Override
	void update() {
		super.update();

		_observers.clear();
	}
}
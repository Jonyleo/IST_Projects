package m19.core;

class RequestNotification extends AbstractNotification {
	RequestNotification(Work work) {
		super("REQUESIÇÃO: ", work);
	}
}
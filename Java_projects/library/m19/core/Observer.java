package m19.core;

import java.io.Serializable;

interface Observer extends Serializable {
	public void notify(String notification);
}
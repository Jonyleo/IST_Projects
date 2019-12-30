package m19.core;

import java.io.Serializable;

import m19.core.exception.CoreRuleFailedException;

abstract class AbstractRule implements Serializable {
	private int _id;

	AbstractRule(int id) {
		_id = id;
	}

	protected CoreRuleFailedException createException() {
		return new CoreRuleFailedException(_id);
	}

	abstract void testRule(Work work, User user) throws CoreRuleFailedException;
}
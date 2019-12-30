package m19.core;

import m19.core.exception.CoreRuleFailedException;

public class NoAvailableCopiesRule extends AbstractRule {
	public NoAvailableCopiesRule(int id) {
		super(id);
	}

	@Override
	void testRule(Work work, User user) throws CoreRuleFailedException {
		if(work.getAvailableCopies() == 0)
			throw createException();
	}
}
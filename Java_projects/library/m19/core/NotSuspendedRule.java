package m19.core;

import m19.core.exception.CoreRuleFailedException;

public class NotSuspendedRule extends AbstractRule {
	public NotSuspendedRule(int id) {
		super(id);
	}

	@Override
	void testRule(Work work, User user) throws CoreRuleFailedException {
		if(user.isSuspended())
			throw createException();
	}

}
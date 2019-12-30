package m19.core;

import m19.core.exception.CoreRuleFailedException;

public class MaxWorksRequestedRule extends AbstractRule {
	public MaxWorksRequestedRule(int id) {
		super(id);
	}

	@Override
	void testRule(Work work, User user) throws CoreRuleFailedException {
		if(user.getNumberOfRequests() == user.getStatus().getMaxWorks())
			throw createException();
	}
}
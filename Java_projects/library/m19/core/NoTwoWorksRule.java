package m19.core;

import m19.core.exception.CoreRuleFailedException;

public class NoTwoWorksRule extends AbstractRule {
	public NoTwoWorksRule(int id) {
		super(id);
	}

	@Override
	void testRule(Work work, User user) throws CoreRuleFailedException {
		if(user.isRequested(work))
			throw createException();
	}
}
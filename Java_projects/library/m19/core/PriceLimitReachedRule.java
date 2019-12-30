package m19.core;

import m19.core.exception.CoreRuleFailedException;

public class PriceLimitReachedRule extends AbstractRule {
	public PriceLimitReachedRule(int id) {
		super(id);
	}

	@Override
	void testRule(Work work, User user) throws CoreRuleFailedException {
		if(! user.getStatus().canRequestWithPrice(work.getPrice()))
			throw createException();
	}
}
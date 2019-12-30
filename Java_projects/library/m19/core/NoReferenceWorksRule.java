package m19.core;

import m19.core.exception.CoreRuleFailedException;

public class NoReferenceWorksRule extends AbstractRule {
	public NoReferenceWorksRule(int id) {
		super(id);
	}

	@Override
	void testRule(Work work, User user) throws CoreRuleFailedException {
		if(work.getCategory() == Category.REFERENCE)
			throw createException();
	}
}
package m19.app.works;

import java.util.List;

import m19.core.LibraryManager;

import m19.core.Work;

import m19.app.works.Message;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.Input;


/**
 * 4.3.3. Perform search according to miscellaneous criteria.
 */
public class DoIncreasePrice extends Command<LibraryManager> {

  private Input<String> _searchTerm;

  /**
   * @param m
   */
  public DoIncreasePrice(LibraryManager m) {
    super("Aumenta preço", m);
    _searchTerm = _form.addStringInput(Message.requestSearchTerm());
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() {
    _form.parse();
    List<String> works = _receiver.doIncreasePrice(_searchTerm.value());
    for(String s : works)
      _display.addLine(s);
    
    _display.addLine("\nForam alteradas " + works.size() + " obras.");
    _display.display();
  }
  
}

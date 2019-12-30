package m19.app.works;

import m19.core.LibraryManager;

import m19.core.Work;

import java.util.List;
import java.util.Comparator;

import pt.tecnico.po.ui.Command;

/**
 * 4.3.2. Display all works.
 */
public class DoDisplayWorks extends Command<LibraryManager> {

  /**
   * @param receiver
   */
  public DoDisplayWorks(LibraryManager receiver) {
    super(Label.SHOW_WORKS, receiver);
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() {
    List<Work> workArray = _receiver.doDisplayWorks();

    workArray.sort(Comparator.comparing(Work::getId));

    for(Work w : workArray) 
      _display.addLine(w.toString());

    _display.display();    
  }
}

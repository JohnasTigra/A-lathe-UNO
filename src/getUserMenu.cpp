
enum pageType (root_menu, run_as_lathe, run_as_step); // all menu page options 
enum pageType currPage = root_menu; // holds current page
// constant port address
const int btn_accept = D8 ; // D8 is K on shield
const int btn_up = D2 ; // D2 is A on shiled 
const int btn_dwn = D4 ; // D4 is C on shield 
const int btn_cancel = D7 ; // D7 is E on shield 

//funciton prototypes
void getUserMenu();

void getUserMenu(){
  switch (currPage){
    case root_menu: page_root_menu(); break; //welcome splash 
    case run_as_lathe: page_run_as_lathe(); break; // X , Z axis in stepping mode and C axis rotating as spindle. 
    case run_as_step: page_run_as_step(); break; // X , Z , C in stepping increment mode.
  }
}
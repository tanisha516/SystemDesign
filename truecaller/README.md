#include<iostream>
using namespace std;

int main() {

    return 0;
}



//Truecaller
FR:
-who is calling
-spam or not -  person can mark spam & know spam.
-upload contact list 


Q:
- how to determine caller details? - same with upload contact list/ someone can upload/ someone in company can upload list 
- merge multiple details for contact.
- one can mark and consolidate on some business logic to determine if spam. - business logic ot in deep. 

FR:
- Person can mark a number spam.
- Person can know spam or not
- Person can know caller details.
- Person can upload contact list. 

API:
- mark Spam
- check Spam
- get Caller Details
- upload Contact details.


(encapsulate instead of store)
(fintune as we go)
(phoneNo is lookup key)
(planned for that part later to distractions) - good point will come to this later.

Classes:




Contact
    - contactNo
    - name
    - isSpam

storageService
    - 

spamService(contactNo)
    - markSpam
    - checkSpam

contactService
    - getContact
    - uploadContacts


service
    





















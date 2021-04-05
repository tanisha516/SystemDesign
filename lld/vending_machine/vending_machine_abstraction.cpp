#include<iostream>
#include<unordered_map>
#include<map>
#include<utility>
#include<vector>
using namespace std;

enum State {
    READY,
    VERIFY_ITEM,
    INSERT_CASH,
    TRANSACTION_COMPLETE,
    END
};

enum class ITEMTYPE {
    PEPSI = 1,
    COKE,
    LAYS
};

enum COINS {
    TEN = 10,
    FIFTY = 50,
    HUNDRED = 100
};

class Item {
    private:
       ITEMTYPE itemType;
       int price;
       int quantity;

    public:
        Item(){}
        Item(ITEMTYPE itemType, int price, int quantity): 
            itemType(itemType), price(price), quantity(quantity){}

        ITEMTYPE getItemtype() {
            return this->itemType;
        }

        int getPrice() {
            return this->price;
        }

        int getQuantity() {
            return this->quantity;
        } 

        void setQuantity(int quantity) {
            this->quantity = quantity;
        }

};

class InventoryService {
    private:
        map<ITEMTYPE, Item> items;
        bool isItemAvailable(ITEMTYPE itemType, int quantity) {
            if (items.find(itemType) != items.end()) {
                if (items[itemType].getQuantity() >= quantity) {
                    return true;
                }
            }
            return false;
        }

    public:
        InventoryService() {
            cout<<" InventoryService called"<<endl;
            this->items.clear();
        }

        void addItems(ITEMTYPE itemType, Item &item) {
            this->items.insert({itemType, item});
            cout<<"addItems "<< (int)itemType << " size "<< this->items.size() <<endl;
        }

        void getItems() {
            cout<< " sizdeee "<< this->items.size() <<endl;
        }

        bool verifyItems(vector<pair<ITEMTYPE, int> >itemsList) {
            for (auto i = 0; i < itemsList.size(); i++) {
                    if(!isItemAvailable(itemsList[i].first, itemsList[i].second)) {
                        cout<< "item not available" << (int)itemsList[i].first << endl;
                        return false;
                    }
            }
            return true;
        }

        int getTotalPrice(vector<pair<ITEMTYPE, int> >itemsList) {
            int totalPrice = 0;
            for (auto i = 0; i < itemsList.size(); i++) {
                    if(isItemAvailable(itemsList[i].first, itemsList[i].second)) {
                        totalPrice += items[itemsList[i].first].getPrice() * itemsList[i].second;
                    } else {
                        cout<< "item not available" << endl;
                        return -1;
                    }
            }
            return totalPrice;
        }

        void dispenseItems(vector<pair<ITEMTYPE, int> >itemsList) {
            for (auto i = 0; i < itemsList.size(); i++) {
                    if(isItemAvailable(itemsList[i].first, itemsList[i].second)) {
                        items[itemsList[i].first].setQuantity(items[itemsList[i].first].getQuantity() - itemsList[i].second);
                    }
            }

            //showInventoryItems();
        }

        void showInventoryItems() {
            cout<< "-----------inventory items ------------" <<endl;
            for (auto it = items.begin(); it != items.end(); it++) {
                cout<< " itemType "<< (int)(*it).first << " quantity "<< (*it).second.getQuantity() <<endl;
            }
            cout<< "-----------------------" <<endl;
        }

};

class Session {
    private:
        State currState;

    public:
        Session() {
            currState = State::READY;
        }

        void cancelSession() {
            currState = State::END;
        }

        void nextState() {
            //cout<< "currState "<< currState << endl;
            switch(currState) {
                case State::READY : 
                    currState = State::VERIFY_ITEM; break;
                case State::VERIFY_ITEM : currState = State::INSERT_CASH; break;
                case State::INSERT_CASH : currState = State::TRANSACTION_COMPLETE; break;
                case State::TRANSACTION_COMPLETE : currState = State::END; break;
                default: currState = State::READY; break;
            }
        }

        State getCurrentState() {
            return currState;
        }
};

class SessionService {
    private:
        Session currentSession;

    public:
        SessionService() {
            Session newSession;
            currentSession = newSession;
        }

        void createSession() {
            Session newSession;
            currentSession = newSession;
        }

        Session& getCurrentSession() {
            return currentSession;
        }

        void endSession() {
            currentSession.cancelSession();
        }
};


class Wallet {
    private:
        map<COINS, int> cash;
    public:
        Wallet(int ten = 0, int fifty = 0, int hundred = 0) {
           cash.insert({COINS::TEN, ten});
           cash.insert({COINS::FIFTY, fifty});
           cash.insert({COINS::HUNDRED, hundred});
       }

       int getTotalWallet() {
           int totalWallet = 0;
           for (auto it = cash.begin(); it != cash.end(); it++) {
               cout<< " wallet coin "<< (*it).first << " cnt "<< (*it).second <<endl;
               totalWallet += (*it).first * (*it).second;  
           }
           return totalWallet;
       }

       void setWallet(COINS coinType, int value) {
           cash[coinType] = value;
       }

       int getCoinCount(COINS coinType) {
           return cash[coinType];
       }
     
};

class WalletService {
    private:
        int vendingMachineWallet = 1000;
        int getChange(int totalChange) {
            vendingMachineWallet -= totalChange;
            return vendingMachineWallet;
        }

        int customerWallet = 0;

    public:
        void resetCustomerWallet() {
            customerWallet = 0;
        }

        void addInCustomerWallet(int cash) {
            customerWallet += cash;
        }

        int getCustomerWallet() {
            return this->customerWallet;
        }

        int getVendingMachineWallet() {
            return vendingMachineWallet;
        }

        bool canPurchaseWithWallet(int expectedAmount) {
            if (customerWallet < expectedAmount) {
                cout<< " insufficient Amount "<< endl;
                return false;
            }
            return true;
        }

        int purchaseAndReturnChange(int expectedAmount) {
            if(!this->canPurchaseWithWallet(expectedAmount)) {
                expectedAmount = 0;
            }

            int customerWalletTotal = this->getCustomerWallet();

            vendingMachineWallet += expectedAmount;
            this->resetCustomerWallet();

            cout<< "Customer wallet " << customerWalletTotal << " Returning amount "<< (customerWalletTotal - expectedAmount) <<endl;
            return (customerWalletTotal - expectedAmount);
        }

};

class VendingMachineBase {
    protected:
        string id;
        SessionService sessionService;
        InventoryService inventoryService;
        WalletService walletService;

    public:
       VendingMachineBase(SessionService &sessionServiceC, InventoryService &inventoryServiceC, WalletService walletServiceC) : 
            sessionService(sessionServiceC), inventoryService(inventoryServiceC), walletService(walletServiceC) {
            }

       SessionService& getSessionService() {
           return this->sessionService;
       }

       InventoryService& getInventoryService() {
           return this->inventoryService;
       }

       void createSession() {
           this->sessionService.createSession();
           this->walletService.resetCustomerWallet();
       }

       void endSession() {
           this->sessionService.endSession();
       }

        virtual bool verifyItems(vector<pair<ITEMTYPE, int> >itemsList) = 0;
        virtual bool insertCash(int totalAmount, vector<pair<ITEMTYPE, int> >itemsList) = 0;
        virtual void finishPurchase(vector<pair<ITEMTYPE, int> >itemsList) = 0;
        virtual void showInventoryItems() = 0;
        
};

//using abstraction: implemented here.
class VendingMachineImp: public VendingMachineBase {
    public:

        VendingMachineImp(SessionService &sessionServiceC, InventoryService &inventoryServiceC, WalletService walletServiceC) : 
                    VendingMachineBase((sessionServiceC), (inventoryServiceC), (walletServiceC)) {
            }
    
       void showInventoryItems() {
           this->inventoryService.showInventoryItems();
       }

       bool verifyItems(vector<pair<ITEMTYPE, int> >itemsList) {
           return this->inventoryService.verifyItems(itemsList);
       }

       bool insertCash(int totalAmount, vector<pair<ITEMTYPE, int> >itemsList) {
           cout<<"inserting cash "<< totalAmount<< endl;
           int expectedAmount = this->inventoryService.getTotalPrice(itemsList);
           this->walletService.addInCustomerWallet(totalAmount);
           if (this->walletService.getCustomerWallet() < expectedAmount) {
               cout << " insufficient wallet "<< this->walletService.getCustomerWallet() << " expected "<< expectedAmount <<endl;
               return false;
           } 
           return true;
       }

       void finishPurchase(vector<pair<ITEMTYPE, int> >itemsList) {
           bool error = false;
           int expectedAmount = this->inventoryService.getTotalPrice(itemsList);
           if (!this->verifyItems(itemsList)) {
               cout<< " Items not found" ;
               expectedAmount = 0;
               this->walletService.purchaseAndReturnChange(expectedAmount);
               return;
           } else if (!this->walletService.canPurchaseWithWallet(expectedAmount)) {
               this->walletService.purchaseAndReturnChange(0);
               return;
           }

           this->inventoryService.dispenseItems(itemsList);
           this->walletService.purchaseAndReturnChange(expectedAmount);
       }        
};

int main() {
    Wallet wallet(5, 1, 1);
    cout<< wallet.getTotalWallet()<<endl;
    cout << wallet.getCoinCount(COINS::FIFTY)<<endl;

    SessionService sessionService;
    InventoryService inventoryService;
    WalletService walletService;

    Item item1(ITEMTYPE::PEPSI, 20, 5);
    Item item2(ITEMTYPE::COKE, 50, 5);

    VendingMachineBase *vendingMachineImpObj = new VendingMachineImp(sessionService, inventoryService, walletService);
    (*vendingMachineImpObj).getInventoryService().addItems(ITEMTYPE::PEPSI, item1);
    (*vendingMachineImpObj).getInventoryService().addItems(ITEMTYPE::COKE, item2);
    //(*vendingMachineImpObj).getInventoryService().addItems(ITEMTYPE::LAYS, item2);
    (*vendingMachineImpObj).showInventoryItems();
    

    // VendingMachineImp vendingMachineImp(sessionService, inventoryService, walletService);
    // vendingMachineImp.getInventoryService().addItems(ITEMTYPE::PEPSI, item1);
    // vendingMachineImp.getInventoryService().addItems(ITEMTYPE::COKE, item2);
    // vendingMachineImp.showInventoryItems();


    int i = 3;
    vector<int> cash({1000,1000, 150, 500, 100});
    while(i--) {
        cout<< "***********************/////////*********************"<<endl;
        State currentSess = (*vendingMachineImpObj).getSessionService().getCurrentSession().getCurrentState();
        if (currentSess == State::END) {
            (*vendingMachineImpObj).createSession();
        } else if (currentSess == State::READY) {

        }   else {
            cout<< " Other session going"<< endl;
            continue;
        }

        vector<pair<ITEMTYPE, int> > itemList;
        itemList.push_back({ITEMTYPE::PEPSI, i+2});
        itemList.push_back({ITEMTYPE::COKE, i+2});


        (*vendingMachineImpObj).getSessionService().getCurrentSession().nextState();
        //cout<<"State "<< (*vendingMachineImpObj).getSessionService().getCurrentSession().getCurrentState()<<endl;
        
        (*vendingMachineImpObj).showInventoryItems();

        cout<< "Items picked "<< (i + 2) <<endl; 
        if (true) {
            (*vendingMachineImpObj).verifyItems(itemList);
            (*vendingMachineImpObj).getSessionService().getCurrentSession().nextState();
            
            (*vendingMachineImpObj).insertCash(cash[i], itemList);
            (*vendingMachineImpObj).getSessionService().getCurrentSession().nextState();

            (*vendingMachineImpObj).finishPurchase(itemList);
            (*vendingMachineImpObj).getSessionService().getCurrentSession().nextState();

        }
        
        (*vendingMachineImpObj).showInventoryItems();
        (*vendingMachineImpObj).endSession();

    }

    return 0;
}

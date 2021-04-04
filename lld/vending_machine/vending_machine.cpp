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

enum ITEMTYPE {
    PEPSI,
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
            cout<<"addItems "<< itemType << " size "<< this->items.size() <<endl;
        }

        void getItems() {
            cout<< " sizdeee "<< this->items.size() <<endl;
        }

        bool verifyItems(vector<pair<ITEMTYPE, int> >itemsList) {
            for (auto i = 0; i < itemsList.size(); i++) {
                    if(!isItemAvailable(itemsList[i].first, itemsList[i].second)) {
                        cout<< "item not available" << itemsList[i].first << endl;
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

            showInventoryItems();
        }

        void showInventoryItems() {
            cout<< "-----------inventory items ------------" <<endl;
            for (auto it = items.begin(); it != items.end(); it++) {
                cout<< " itemType "<< (*it).first << " quantity "<< (*it).second.getQuantity() <<endl;
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
            cout<< "currState "<< currState << endl;
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

        int customerWallet;

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

class VendingMachine {
    private:
        string id;
        SessionService sessionService;
        InventoryService inventoryService;
        WalletService walletService;

    public:
       VendingMachine(SessionService &sessionServiceC, InventoryService &inventoryServiceC, WalletService walletServiceC) : 
            sessionService(sessionServiceC), inventoryService(inventoryServiceC), walletService(walletServiceC) {
                this->inventoryService = inventoryServiceC;
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


       bool verifyItems(vector<pair<ITEMTYPE, int> >itemsList) {
           return this->inventoryService.verifyItems(itemsList);
       }

       bool insertCash(int totalAmount, vector<pair<ITEMTYPE, int> >itemsList) {
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


       void showInventoryItems() {
           this->inventoryService.showInventoryItems();
       }
        
};

int main() {

    SessionService sessionService;
    InventoryService inventoryService;
    WalletService walletService;

    VendingMachine vendingMachine(sessionService, inventoryService, walletService);

    Item item1(ITEMTYPE::PEPSI, 20, 5);
    Item item2(ITEMTYPE::COKE, 50, 5);
    vendingMachine.getInventoryService().addItems(ITEMTYPE::PEPSI, item1);
    vendingMachine.getInventoryService().addItems(ITEMTYPE::COKE, item2);

    int i = 3;
    vector<int> cash({1000, 150, 5});
    while(i--) {
        cout<< "***********************"<<endl;
        State currentSess = vendingMachine.getSessionService().getCurrentSession().getCurrentState();
        if (currentSess == State::END) {
            vendingMachine.createSession();
        } else if (currentSess == State::READY) {

        }   else {
            cout<< " Other session going"<< endl;
            continue;
        }

        vector<pair<ITEMTYPE, int> > itemList;
        itemList.push_back({ITEMTYPE::PEPSI, i+1});
        itemList.push_back({ITEMTYPE::COKE, i+1});


        vendingMachine.getSessionService().getCurrentSession().nextState();
        cout<<"State "<< vendingMachine.getSessionService().getCurrentSession().getCurrentState()<<endl;
        
        vendingMachine.showInventoryItems();

        if (vendingMachine.verifyItems(itemList)) {
            vendingMachine.getSessionService().getCurrentSession().nextState();
            
            vendingMachine.insertCash(cash[i], itemList);
            vendingMachine.getSessionService().getCurrentSession().nextState();

            vendingMachine.finishPurchase(itemList);
            vendingMachine.getSessionService().getCurrentSession().nextState();

        }
        
        vendingMachine.showInventoryItems();

        vendingMachine.endSession();


    }


    return 0;
}

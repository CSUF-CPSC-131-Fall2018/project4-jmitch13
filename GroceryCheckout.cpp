#include	<iostream>
#include    <stdexcept>
#include	"GroceryCheckout.h"

using namespace std;

bool GroceryInventory::AddItem(const string& name, int quantity,
							   float price, bool taxable)
{
    //Checks to see if the item has already been added
	if (item_.find(name) != item_.end())
        return false;
    else
    //Adds the item if it was not found
        item_[name] = GroceryItem(name, quantity, price, taxable);
        return true;
}

void GroceryInventory::CreateFromFile(const string& fileName)
{
    ifstream	file(fileName);
	string		name;
	float		price;
	int			quantity;
	bool		taxable;
	
	if (file.is_open())
	{
		while (true)
		{
			file >> name >> quantity >> price >> taxable;
			if (!file.fail())
			{
				AddItem(name, quantity, price, taxable);
			}
			else
			{
				break;
			}
         }
        file.close();
    }
	else
	{
        cout << "Could not open file " + fileName << endl;
    }
	
	return;
}

Receipt GroceryInventory::CreateReceipt(const string& fileName)
{
    ifstream   file(fileName);
    string   name;
    Receipt   receipt;
    receipt.subtotal_ = receipt.taxAmount_ = receipt.total_ = 0.0;
    if (file.is_open())
    {
        while (true)
        {
            //Checks to see if the file can still keep reading
            file >> name;
            if (!file.fail())
            {
                //Create a grocery item object
                GroceryItem* gi = FindItem(name);
                if(gi != nullptr)
                {
                    receipt.item_.push_back(ReceiptItem(name, gi->price_));
                    //update total
                    receipt.subtotal_ += gi->price_;
                    //Checks to see if it is taxable
                    if(gi->taxable_)
                    {
                        receipt.taxAmount_ += taxRate_ / 100.0 * gi->price_;
                    }
                    //update inventory
                    gi->quantity_ --;
                }
            }
            else
            {
                break;
            }
        }
        file.close();
    }
    else
    {
        cout << "Could not open file " + fileName << endl;
    }
    //Creates the receipt
    receipt.total_ = receipt.subtotal_ + receipt.taxAmount_;
    return receipt;
}

GroceryItem* GroceryInventory::FindItem(const string& name)
{
    //Returns a nullptr if the item is not found
	if (item_.find(name) == item_.end())
        return nullptr;
    //Returns the found item
    else
        return &(item_[name]); //derefrences
}

bool GroceryInventory::RemoveItem(const string& name)
{
    //Checks to see if the item exists
	if (item_.find(name) == item_.end())
        return false;
    //Removes the item
    else
        item_.erase(name);
        return true;
}
//Sets the taxRate_ variable created earlier to equal the taxRate
void GroceryInventory::SetTaxRate(float taxRate)
{
    taxRate_ = taxRate;
}
//returns the size of the map
size_t GroceryInventory::Size()
{
    return item_.size();
}

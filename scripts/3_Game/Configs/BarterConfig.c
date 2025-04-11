class BarterInfo
{
    string classname;
    int worth;
    int type;

    void BarterInfo(string item, int amount, int itemtype)
    {
        classname = item;
        worth = amount;
        type = itemtype;
    }
}

class BarterConfig
{
    static BarterInfo GetBarterItemByClassname(string classname)
    {
        array<ref BarterInfo> barterItems = GetAllBarterItems();

        foreach (BarterInfo item : barterItems)
        {
            if (item.classname == classname)
            {
                return item;
            }
        }

        return null;
    }

    static ref array<ref BarterInfo> GetAllBarterItems()
    {
        return {
            new BarterInfo("Cannabis", 1000, 0),
        };
    }
}
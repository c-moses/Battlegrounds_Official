class KillfeedWrapper
{
	private Widget root;
	private ref array<ref KillfeedEntry> items;

	void KillfeedWrapper()
	{
		root = GetGame().GetWorkspace().CreateWidgets("Battlegrounds/GUI/layouts/KillfeedWrapper.layout", null);
		items = new array<ref KillfeedEntry>();
	}

	Widget GetRoot()
	{
		return root;
	}

	void AddItem(Param8<string, string, string, string, string, int, string, int> data)
	{
		items.Insert(new KillfeedEntry(this, data.param1, data.param2, data.param3, data.param4, data.param5, data.param6, data.param7, data.param8));
	}

	void RemoveItem(KillfeedEntry item)
	{
		int idx = items.Find(item);
		if (idx + 1)
		{
			items.Remove(idx);
		}
	}
}
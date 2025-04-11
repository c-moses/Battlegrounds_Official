class SafezoneLocation
{
    int     id;
    int     radius;
    vector  position;
    string  name;

    void SafezoneLocation(int szid, int szradius, vector szposition, string szname)
    {
        this.id = szid;
        this.radius = szradius;
        this.position = szposition;
        this.name = szname;
    }
}

class SafezoneLocations
{
    private static ref SafezoneLocations instance;
    ref array<ref SafezoneLocation> locations;

    void SafezoneLocations()
    {
        locations = new array<ref SafezoneLocation>();
    }

    void AddSafezone(int id, int radius, vector position, string name)
    {
        locations.Insert(new SafezoneLocation(id, radius, position, name));
    }

    array<ref SafezoneLocation> GetSafezones()
    {
        return locations;
    }

    void ClearAllSafezones()
    {
        locations.Clear();
    }

    static SafezoneLocations GetInstance()
    {
        if (!instance)
        {
            instance = new SafezoneLocations();
        }
        return instance;
    }
}
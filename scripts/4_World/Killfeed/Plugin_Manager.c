modded class PluginManager
{
	override void Init()
	{
		super.Init();
		RegisterPlugin("Killfeed", false, true);
	}
}
class ServerRPC
{
    private static const string SERVER_RPC_FOLDER = "$profile:\\Battlegrounds\\ServerRPC"; // outbox
    private static const string GAME_RPC_FOLDER = "$profile:\\Battlegrounds\\GameRPC"; // inbox

    static void InitServerRPCs()
    {
        if (FileExist(SERVER_RPC_FOLDER))
        {
            DeleteFolderContents(SERVER_RPC_FOLDER);
        }
        else
        {
            MakeDirectory(SERVER_RPC_FOLDER);
        }
    }

    static void SendServerRPC(string rpcType, string param1)
    {
        if (rpcType == "" || param1 == "")
            return;

        string fileName = SERVER_RPC_FOLDER + "\\" + rpcType + ".txt";
        FileHandle handle = OpenFile(fileName, FileMode.WRITE);
        if (handle != 0)
        {
            FPrint(handle, param1);
            CloseFile(handle);
        }
    }

    private static void DeleteFolderContents(string folderPath)
    {
        string fileName;
        FileAttr fileAttributes;
        FindFileHandle handle = FindFile(folderPath + "/*", fileName, fileAttributes, 0);

        if (handle == 0)
            return;

        bool hasMoreFiles = true;

        while (hasMoreFiles)
        {
            string fullPath = folderPath + "/" + fileName;

            if (FileExist(fullPath))
            {
                DeleteFile(fullPath);
            }

            hasMoreFiles = FindNextFile(handle, fileName, fileAttributes);
        }

        CloseFindFile(handle);
    }
}
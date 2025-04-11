class LevelManager
{
    private ref array<int> levelRequirements;

    void LevelManager()
    {
        levelRequirements = new array<int>;

        levelRequirements.Insert(3000);    // Level 1
        levelRequirements.Insert(6000);    // Level 2
        levelRequirements.Insert(9500);    // Level 3
        levelRequirements.Insert(13500);   // Level 4
        levelRequirements.Insert(18000);   // Level 5
        levelRequirements.Insert(23000);   // Level 6
        levelRequirements.Insert(28500);   // Level 7
        levelRequirements.Insert(34500);   // Level 8
        levelRequirements.Insert(41000);   // Level 9
        levelRequirements.Insert(48000);   // Level 10
        levelRequirements.Insert(55500);   // Level 11
        levelRequirements.Insert(63500);   // Level 12
        levelRequirements.Insert(72000);   // Level 13
        levelRequirements.Insert(81000);   // Level 14
        levelRequirements.Insert(90500);   // Level 15
        levelRequirements.Insert(100500);  // Level 16
        levelRequirements.Insert(111000);  // Level 17
        levelRequirements.Insert(122000);  // Level 18
        levelRequirements.Insert(133500);  // Level 19
        levelRequirements.Insert(145500);  // Level 20
        levelRequirements.Insert(158000);  // Level 21
        levelRequirements.Insert(171000);  // Level 22
        levelRequirements.Insert(184500);  // Level 23
        levelRequirements.Insert(198500);  // Level 24
        levelRequirements.Insert(213000);  // Level 25
        levelRequirements.Insert(228000);  // Level 26
        levelRequirements.Insert(243500);  // Level 27
        levelRequirements.Insert(259500);  // Level 28
        levelRequirements.Insert(276000);  // Level 29
        levelRequirements.Insert(293000);  // Level 30
        levelRequirements.Insert(310500);  // Level 31
        levelRequirements.Insert(328500);  // Level 32
        levelRequirements.Insert(347000);  // Level 33
        levelRequirements.Insert(366000);  // Level 34
        levelRequirements.Insert(385500);  // Level 35
        levelRequirements.Insert(405500);  // Level 36
        levelRequirements.Insert(426000);  // Level 37
        levelRequirements.Insert(447000);  // Level 38
        levelRequirements.Insert(468500);  // Level 39
        levelRequirements.Insert(490500);  // Level 40
        levelRequirements.Insert(513000);  // Level 41
        levelRequirements.Insert(536000);  // Level 42
        levelRequirements.Insert(559500);  // Level 43
        levelRequirements.Insert(583500);  // Level 44
        levelRequirements.Insert(608000);  // Level 45
        levelRequirements.Insert(633000);  // Level 46
        levelRequirements.Insert(658500);  // Level 47
        levelRequirements.Insert(684500);  // Level 48
        levelRequirements.Insert(711000);  // Level 49
        levelRequirements.Insert(738000);  // Level 50
        levelRequirements.Insert(765500);  // Level 51
        levelRequirements.Insert(793500);  // Level 52
        levelRequirements.Insert(822000);  // Level 53
        levelRequirements.Insert(851000);  // Level 54
        levelRequirements.Insert(880500);  // Level 55
        levelRequirements.Insert(910500);  // Level 56
        levelRequirements.Insert(941000);  // Level 57
        levelRequirements.Insert(972000);  // Level 58
        levelRequirements.Insert(1003500); // Level 59
        levelRequirements.Insert(1035500); // Level 60
        levelRequirements.Insert(1068000); // Level 61
        levelRequirements.Insert(1101000); // Level 62
        levelRequirements.Insert(1134500); // Level 63
        levelRequirements.Insert(1168500); // Level 64
        levelRequirements.Insert(1203000); // Level 65
        levelRequirements.Insert(1238000); // Level 66
        levelRequirements.Insert(1273500); // Level 67
        levelRequirements.Insert(1309500); // Level 68
        levelRequirements.Insert(1346000); // Level 69
        levelRequirements.Insert(1383000); // Level 70
        levelRequirements.Insert(1420500); // Level 71
        levelRequirements.Insert(1458500); // Level 72
        levelRequirements.Insert(1497000); // Level 73
        levelRequirements.Insert(1536000); // Level 74
        levelRequirements.Insert(1575500); // Level 75
        levelRequirements.Insert(1615500); // Level 76
        levelRequirements.Insert(1656000); // Level 77
        levelRequirements.Insert(1697000); // Level 78
        levelRequirements.Insert(1738500); // Level 79
        levelRequirements.Insert(1780500); // Level 80
        levelRequirements.Insert(1823000); // Level 81
        levelRequirements.Insert(1866000); // Level 82
        levelRequirements.Insert(1909500); // Level 83
        levelRequirements.Insert(1953500); // Level 84
        levelRequirements.Insert(1998000); // Level 85
        levelRequirements.Insert(2043000); // Level 86
        levelRequirements.Insert(2088500); // Level 87
        levelRequirements.Insert(2134500); // Level 88
        levelRequirements.Insert(2181000); // Level 89
        levelRequirements.Insert(2228000); // Level 90
        levelRequirements.Insert(2275500); // Level 91
        levelRequirements.Insert(2323500); // Level 92
        levelRequirements.Insert(2372000); // Level 93
        levelRequirements.Insert(2421000); // Level 94
        levelRequirements.Insert(2470500); // Level 95
        levelRequirements.Insert(2520500); // Level 96
        levelRequirements.Insert(2571000); // Level 97
        levelRequirements.Insert(2622000); // Level 98
        levelRequirements.Insert(2673500); // Level 99
        levelRequirements.Insert(2725500); // Level 100
        levelRequirements.Insert(2778000); // Level 101
        levelRequirements.Insert(2831000); // Level 102
        levelRequirements.Insert(2884500); // Level 103
        levelRequirements.Insert(2938500); // Level 104
        levelRequirements.Insert(2993000); // Level 105
        levelRequirements.Insert(3048000); // Level 106
        levelRequirements.Insert(3103500); // Level 107
        levelRequirements.Insert(3159500); // Level 108
        levelRequirements.Insert(3216000); // Level 109
        levelRequirements.Insert(3273000); // Level 110
        levelRequirements.Insert(3330500); // Level 111
        levelRequirements.Insert(3388500); // Level 112
        levelRequirements.Insert(3447000); // Level 113
        levelRequirements.Insert(3506000); // Level 114
        levelRequirements.Insert(3565500); // Level 115
        levelRequirements.Insert(3625500); // Level 116
        levelRequirements.Insert(3686000); // Level 117
        levelRequirements.Insert(3747000); // Level 118
        levelRequirements.Insert(3808500); // Level 119
        levelRequirements.Insert(3870500); // Level 120
        levelRequirements.Insert(3933000); // Level 121
        levelRequirements.Insert(3996000); // Level 122
        levelRequirements.Insert(4059500); // Level 123
        levelRequirements.Insert(4123500); // Level 124
        levelRequirements.Insert(4188000); // Level 125
        levelRequirements.Insert(4253000); // Level 126
        levelRequirements.Insert(4318500); // Level 127
        levelRequirements.Insert(4384500); // Level 128
        levelRequirements.Insert(4451000); // Level 129
        levelRequirements.Insert(4518000); // Level 130
        levelRequirements.Insert(4585500); // Level 131
        levelRequirements.Insert(4653500); // Level 132
        levelRequirements.Insert(4722000); // Level 133
        levelRequirements.Insert(4791000); // Level 134
        levelRequirements.Insert(4860500); // Level 135
        levelRequirements.Insert(4930500); // Level 136
        levelRequirements.Insert(5001000); // Level 137
        levelRequirements.Insert(5072000); // Level 138
        levelRequirements.Insert(5143500); // Level 139
        levelRequirements.Insert(5215500); // Level 140
        levelRequirements.Insert(5288000); // Level 141
        levelRequirements.Insert(5361000); // Level 142
        levelRequirements.Insert(5434500); // Level 143
        levelRequirements.Insert(5508500); // Level 144
        levelRequirements.Insert(5583000); // Level 145
        levelRequirements.Insert(5658000); // Level 146
        levelRequirements.Insert(5733500); // Level 147
        levelRequirements.Insert(5809500); // Level 148
        levelRequirements.Insert(5886000); // Level 149
        levelRequirements.Insert(5963000); // Level 150
    }

    int GetPlayerLevel(int score)
    {
        for (int i = 0; i < levelRequirements.Count(); i++)
        {
            if (score < levelRequirements[i])
            {
                return i;
            }
        }

        return levelRequirements.Count(); // If score exceeds the last threshold, return max level
    }

    int GetXPForLevel(int level)
    {
        if (level > 0 && level <= levelRequirements.Count())
        {
            return levelRequirements[level - 1];
        }
        return 0;
    }

    int GetXPForNextLevel(int level)
    {
        if (level >= 0 && level < levelRequirements.Count())
        {
            return levelRequirements[level];
        }
        return -1;
    }
}
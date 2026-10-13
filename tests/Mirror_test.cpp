// SPDX-License-Identifier: GPL-3.0-only
#include <QTest>

#include <QUrl>
#include "net/Mirror.h"

class MirrorTest : public QObject {
    Q_OBJECT
   private slots:
    void test_emptyBase_unchanged()
    {
        QUrl in(QStringLiteral("https://launchermeta.mojang.com/mc/game/version_manifest_v2.json"));
        QUrl out = Net::mirrorUrl(in, QString());
        QCOMPARE(out, in);
    }

    void test_allowlisted_rewritten()
    {
        QUrl in(QStringLiteral("https://launchermeta.mojang.com/mc/game/version_manifest_v2.json"));
        QUrl out = Net::mirrorUrl(in, QStringLiteral("https://mcassets.ifbw.uk"));
        QCOMPARE(out.toString(),
                 QStringLiteral("https://mcassets.ifbw.uk/launchermeta.mojang.com/mc/game/version_manifest_v2.json"));
    }

    void test_nonAllowlisted_unchanged()
    {
        QUrl in(QStringLiteral("https://example.com/some/path"));
        QUrl out = Net::mirrorUrl(in, QStringLiteral("https://mcassets.ifbw.uk"));
        QCOMPARE(out, in);
    }

    void test_authHost_unchanged()
    {
        // login/auth hosts must stay untouched and are never rewritten
        const QStringList authUrls = {
            QStringLiteral("https://login.microsoftonline.com/consumers/oauth2/v2.0/token"),
            QStringLiteral("https://api.minecraftservices.com/minecraft/profile"),
            QStringLiteral("https://sessionserver.mojang.com/session/minecraft/profile/abc"),
            QStringLiteral("https://textures.minecraft.net/texture/abc"),
        };
        for (const auto& s : authUrls) {
            QUrl in(s);
            QUrl out = Net::mirrorUrl(in, QStringLiteral("https://mcassets.ifbw.uk"));
            QCOMPARE(out, in);
        }
    }

    void test_queryKept()
    {
        QUrl in(QStringLiteral("https://api.modrinth.com/v2/search?query=fabric&limit=1"));
        QUrl out = Net::mirrorUrl(in, QStringLiteral("https://mcassets.ifbw.uk"));
        QCOMPARE(out.host(), QStringLiteral("mcassets.ifbw.uk"));
        QCOMPARE(out.path(), QStringLiteral("/api.modrinth.com/v2/search"));
        QVERIFY(out.hasQuery());
        QCOMPARE(out.query(), in.query());
        QCOMPARE(out.toString(),
                 QStringLiteral("https://mcassets.ifbw.uk/api.modrinth.com/v2/search?query=fabric&limit=1"));
    }

    void test_allAllowlistedHosts_rewritten()
    {
        const QStringList hosts = {
            QStringLiteral("resources.download.minecraft.net"),
            QStringLiteral("libraries.minecraft.net"),
            QStringLiteral("launchermeta.mojang.com"),
            QStringLiteral("launcher.mojang.com"),
            QStringLiteral("piston-meta.mojang.com"),
            QStringLiteral("piston-data.mojang.com"),
            QStringLiteral("meta.fabricmc.net"),
            QStringLiteral("maven.fabricmc.net"),
            QStringLiteral("meta.prismlauncher.org"),
            QStringLiteral("api.modrinth.com"),
            QStringLiteral("cdn.modrinth.com"),
        };
        for (const auto& h : hosts) {
            QUrl in(QStringLiteral("https://%1/some/path").arg(h));
            QUrl out = Net::mirrorUrl(in, QStringLiteral("https://mcassets.ifbw.uk"));
            QCOMPARE(out.host(), QStringLiteral("mcassets.ifbw.uk"));
            QVERIFY(out.path().startsWith(QStringLiteral("/") + h + QStringLiteral("/")));
        }
    }
};

QTEST_GUILESS_MAIN(MirrorTest)

#include "Mirror_test.moc"

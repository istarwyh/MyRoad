我们想要把自己的开源库发布到Maven的中央仓库是可以的，但是不能直接上传，需要有中转站类似[central.sonatype.org](https://central.sonatype.org/)，它会在检查核实了该依赖没有问题后，定期自动同步到Maven的中央仓库。上传到 [central.sonatype.org](https://central.sonatype.org/) 还有两种方式：
1. Publishing via the Central Portal
2. Publishing via OSSRH
## 推荐的做法
原本通常的做法就是第二种，第一种其实是2024年才推出的，目前也是推荐的发布方式。官方是希望通过[API](https://central.sonatype.org/publish/publish-portal-api/)，Publishing By Uploading a Bundle[⚓︎](https://central.sonatype.org/publish/publish-portal-upload/#publishing-by-uploading-a-bundle "Permanent link") 。不过当前简单说就是**手动**上传符合他要求的包。之所以是手动，因为 sonatype 那边希望切换原本上传到 OSSRH 的方式到走 [central.sonatype.com](https://central.sonatype.com)发布API的方式，但是新的平台能力目前只建设到了手动上传的地步。
而原有上传到 OSSRH 需要 token,现在想申请还是有点麻烦的，还需要说明你为啥不能用新的方式[^trouble]。
关系就是这样，但是看文档的时候可能会因为很多名词而困惑，这里简单补充几个基本名词：

- [central.sonatype.org](https://central.sonatype.org/)： 面向开源的一个Nexus公共服务
- [Nexus](https://www.sonatype.com/nexus-repository-oss)：一个支持Maven仓库的软件，由Sonatype开发，有免费版和专业版两个版本，很多大公司内部都使用Nexus作为自己的私有Maven仓库。
- OSSRH：Open Source Software Repository Hosting,即开源软件仓库托管
- Central Portal: 指 central.sonatype.com ，既指其网页端，也指其API
- NXRM: Nexus Repository Manager,即 Nexus 仓库管理，原本用于产生访问 OSSRH token的地方
- [`issues.sonatype.org`](https://central.sonatype.org/news/20240109_issues_sonatype_org_deprecation/)： 原本面向用户申请 token的地方，已经被废弃


## 具体步骤

[^trouble]:https://central.sonatype.org/register/legacy/
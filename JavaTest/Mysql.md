GRANT SELECT 
ON TABLE 职工,部门
TO 王明;

GRANT SELECT
ON TABLE 职工,部门
TO 李勇;

GRANT SELECT
ON TABLE 职工 WHEN USER() = NAME
--只对自己的记录,在TABLE 职工里
TO ALL;
--这里假定这个DBMS支持when与user()的使用

GRANT SELECT,UPDATE(工资)
ON TABLE 职工
--这里的工资是字段,但都在职工这张表上
TO 刘星;

GRANT ALTER TABLE
--修改表的结构
ON TABLE 职工,部门
TO 张新; 

GRANT ALL PRIVILEGES
ON TABLE 职工,部门
WITH GRANT OPTIONS
TO 周平;

--这么细致的定义存取权限,需要通过定义一个选择好后的视图,然后把这个视图的权限给用户
CREATE VIEW 部门工资(部门号,名称,最高工资,最低工资,平均工资)
AS 
SELECT 部门号,名称,max(工资),min(工资),AVG(工资)
FROM TABLE 职工,部门
WHERE 职工.部门号 = 部门.部门号
GROUP BY 职工.部门号;
--注意这里有个分号,表创建完毕
--并指明按照 职工.部门号 进行排序
GRANT SELECT ON TABLE 部门工资
--注意这里不是view,还是table    
TO 杨兰;

----------------------------------------------------------------
CREATE TABLE 部门
    (部门号 SMALLINT PRIMARY KEY,
    --没有int,只有smallint 与 number么 
     名称   char(8),
     经理名 char(20),
     电话   char(20)
    );
    --这里必须有逗号,因为是部门内部的关系吧
CREATE TABLE 职工
    (职工号 smallint primary key,
     姓名   char(20),
     年龄   number(2),
     职务   char(10),
     工资   number(8,2),
     部门号 smallint,
     foreign key(部门号) references 部门(部门号),
     constraint C1 check(年龄 <= 60),
     --constraint 是单独拿出来check,否则直接check也可以
    );

S(Sno,Sname,Sbirth,Dept,Class,Rno)
C(Class,Cmno,Dept,Cnum,Cyear)
D(Dept,Dno,Dpos,Dnum)
M(Mname,Myear,Mpos,Mnum)

$Sno \to Sname, Sno \to Sbirth,Sno \to Class,Class \to Dept,Dept \to Rno$
$Sno \to Class,Class \to Dept,Class \nrightarrow Sno$ Sno Dept
$Sno \to Dept,Dept \to Rno,Dept \nrightarrow Sno$ Sno Rno
$Class \to Dept,Dept \to Rno,Dept \nrightarrow Class$ Class Rno

$Class \to Cmno,Cmno \to Dept,Class \to Cnum,Class \to Cyear$
$Class \to Cmno,Cmno \to Dept,Cmno \nrightarrow Class$ Class Dept

$Dept \to Dno,Dno \to Dept,Dno \to Dpos,Dno \to Dnum$
因为$Dno \to Dept$,所以Dept 对Dno,Dpos,Dnum都没有传递依赖关系

$Mname \to Myear,Mname \to Mpos,Mname \to Mnum$
该模式没有传递依赖关系

各关系模式的候选码,外部码,全码用"|"分割如下:
- Sno | Dept,Class | NULL
- Class | Dept | NULL
- Dept 或 Dno | NULL | NULL
- Mname | NULL | NULL

$X \to Z,X \to Y,则有 X \to YZ$
$X \to Y,WY\to Z,则有 WX \to Z$
$X \to Y,Z \subset Y,则有 X \to Z$
import sqlite3
import rsa  # pip install rsa


class Database:
	def __init__(self):
		self.__connection = sqlite3.connect("system.db")
		self.cursor = self.__connection.cursor()
		self.cursor.execute("""CREATE TABLE IF NOT EXISTS cardholders(
			uid BLOB PRIMARY KEY,
			bal SMALLINT,
			full_name TEXT,
			person_id TEXT,
			clearance int,
			counterval tinyint
		)""")
		with open("public.pem", "rb") as f:
			data = f.read()
		self.publkey = rsa.PublicKey.load_pkcs1(data)
		with open("private.pem", "rb") as f:
			data = f.read()
		self.privkey = rsa.PrivateKey.load_pkcs1(data)

	def __del__(self):
		self.__connection.close()

	def get_bal_from_uid(self, uid):
		cuid = rsa.sign(str(uid).encode("utf-8"), self.privkey, 'SHA-256')
		self.cursor.execute("SELECT bal, full_name FROM cardholders WHERE uid=?", [cuid])
		fetch = self.cursor.fetchone()
		print(fetch[0], rsa.decrypt(fetch[1], self.privkey))

	def insert(self):
		cuid = rsa.sign(str(1234).encode("utf-8"), self.privkey, 'SHA-256')
		cname = rsa.encrypt("Melle Bosboom".encode('utf-8'), self.publkey)
		self.cursor.execute("""INSERT INTO cardholders VALUES (?, 2000, ?, 42069, 69, 420)
		""", [cuid, cname])
